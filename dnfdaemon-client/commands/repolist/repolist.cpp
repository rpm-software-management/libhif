/*
Copyright (C) 2020 Red Hat, Inc.

This file is part of dnfdaemon-client: https://github.com/rpm-software-management/libdnf/

Dnfdaemon-client is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Dnfdaemon-client is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with dnfdaemon-client.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "repolist.hpp"

#include "../../context.hpp"

#include <dnfdaemon-server/dbus.hpp>
#include <libdnf/conf/option_string.hpp>

#include "libdnf-cli/output/repoinfo.hpp"
#include "libdnf-cli/output/repolist.hpp"

#include <iostream>
#include <memory>
#include <vector>

namespace dnfdaemon::client {

void CmdRepolist::set_argument_parser(Context & ctx) {
    enable_disable_option = dynamic_cast<libdnf::OptionEnum<std::string> *>(
        ctx.arg_parser.add_init_value(std::unique_ptr<libdnf::OptionEnum<std::string>>(
            new libdnf::OptionEnum<std::string>("enabled", {"all", "enabled", "disabled"}))));

    auto all = ctx.arg_parser.add_new_named_arg("all");
    all->set_long_name("all");
    all->set_short_description("show all repos");
    all->set_const_value("all");
    all->link_value(enable_disable_option);

    auto enabled = ctx.arg_parser.add_new_named_arg("enabled");
    enabled->set_long_name("enabled");
    enabled->set_short_description("show enabled repos (default)");
    enabled->set_const_value("enabled");
    enabled->link_value(enable_disable_option);

    auto disabled = ctx.arg_parser.add_new_named_arg("disabled");
    disabled->set_long_name("disabled");
    disabled->set_short_description("show disabled repos");
    disabled->set_const_value("disabled");
    disabled->link_value(enable_disable_option);

    patterns_to_show_options = ctx.arg_parser.add_new_values();
    auto repos = ctx.arg_parser.add_new_positional_arg(
        "repos_to_show",
        libdnf::cli::ArgumentParser::PositionalArg::UNLIMITED,
        ctx.arg_parser.add_init_value(std::unique_ptr<libdnf::Option>(new libdnf::OptionString(nullptr))),
        patterns_to_show_options);
    repos->set_short_description("List of repos to show");

    auto conflict_args =
        ctx.arg_parser.add_conflict_args_group(std::unique_ptr<std::vector<libdnf::cli::ArgumentParser::Argument *>>(
            new std::vector<libdnf::cli::ArgumentParser::Argument *>{all, enabled, disabled}));

    all->set_conflict_arguments(conflict_args);
    enabled->set_conflict_arguments(conflict_args);
    disabled->set_conflict_arguments(conflict_args);

    auto repolist = ctx.arg_parser.add_new_command(command);
    repolist->set_short_description("display the configured software repositories");
    repolist->set_description("");
    repolist->set_named_args_help_header("Optional arguments:");
    repolist->set_positional_args_help_header("Positional arguments:");
    repolist->set_parse_hook_func([this, &ctx](
                                      [[maybe_unused]] libdnf::cli::ArgumentParser::Argument * arg,
                                      [[maybe_unused]] const char * option,
                                      [[maybe_unused]] int argc,
                                      [[maybe_unused]] const char * const argv[]) {
        ctx.select_command(this);
        return true;
    });

    repolist->register_named_arg(all);
    repolist->register_named_arg(enabled);
    repolist->register_named_arg(disabled);
    repolist->register_positional_arg(repos);

    ctx.arg_parser.get_root_command()->register_command(repolist);
}

class RepoDbus {
public:
    RepoDbus(dnfdaemon::KeyValueMap & rawdata) : rawdata(rawdata){};
    std::string get_id() { return rawdata["id"]; }
    std::string get_name() { return rawdata["name"]; }
    bool is_enabled() { return rawdata["enabled"]; }
    uint64_t get_size() { return rawdata["repo_size"]; }

private:
    dnfdaemon::KeyValueMap rawdata;
};

class RepoDbusList {
public:
    RepoDbusList(dnfdaemon::KeyValueMapList & rawlist) : rawlist(rawlist){};

    std::vector<std::unique_ptr<RepoDbus>> get_data() {
        std::vector<std::unique_ptr<RepoDbus>> vec;
        for ( auto & rawdata : rawlist ) {
            vec.push_back(std::make_unique<RepoDbus>(rawdata));
        }
    return vec;
    };

private:
    dnfdaemon::KeyValueMapList rawlist;
};

void CmdRepolist::run(Context & ctx) {
    // prepare options from command line arguments
    dnfdaemon::KeyValueMap options = {};
    options["enable_disable"] = enable_disable_option->get_value();
    std::vector<std::string> patterns_to_show;
    if (patterns_to_show_options->size() > 0) {
        patterns_to_show.reserve(patterns_to_show_options->size());
        for (auto & pattern : *patterns_to_show_options) {
            patterns_to_show.emplace_back(dynamic_cast<libdnf::OptionString *>(pattern.get())->get_value());
        }
    }
    options["patterns_to_show"] = patterns_to_show;

    options["command"] = command;

    if (command == "repoinfo") {
        // load repositories
        if (ctx.wait_for_repos() == Context::RepoStatus::ERROR) {
            throw std::runtime_error("Failed to load repositories");
        };
    }

    // call list() method on repo interface via dbus
    dnfdaemon::KeyValueMapList repositories;
    ctx.session_proxy->callMethod("list")
        .onInterface(dnfdaemon::INTERFACE_REPO)
        .withArguments(options)
        .storeResultsTo(repositories);

    if (command == "repolist") {
        // print the output table
        bool with_status = enable_disable_option->get_value() == "all";
        libdnf::cli::output::print_repolist_table(
            RepoDbusList(repositories),
            with_status,
            libdnf::cli::output::COL_REPO_ID);
   } else {
        // repoinfo command
        // TODO(mblaha): output using smartcols
        /*for (auto & raw_repo : repositories) {
            RepoDbus repo(raw_repo);
            std::cout << "REPO: " << repo.get_id() << std::endl;
            std::cout << "size: " << repo.get_size() << std::endl;
        }*/
        libdnf::cli::output::print_repoinfo_table(RepoDbusList(repositories));
    }
}

}  // namespace dnfdaemon::client
