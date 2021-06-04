// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "grouplist.hpp"

#include "../../context.hpp"

#include <libdnf/comps/comps.hpp>
#include <libdnf/comps/group/group.hpp>
#include <libdnf/comps/group/query.hpp>
#include "libdnf-cli/output/grouplist.hpp"

#include <filesystem>
#include <fstream>
#include <set>


namespace microdnf {


using namespace libdnf::cli;
void CmdGrouplist::set_argument_parser(Context & ctx) {
    available_option = dynamic_cast<libdnf::OptionBool *>(
        ctx.arg_parser.add_init_value(std::unique_ptr<libdnf::OptionBool>(new libdnf::OptionBool(false))));

    installed_option = dynamic_cast<libdnf::OptionBool *>(
        ctx.arg_parser.add_init_value(std::unique_ptr<libdnf::OptionBool>(new libdnf::OptionBool(false))));

    hidden_option = dynamic_cast<libdnf::OptionBool *>(
        ctx.arg_parser.add_init_value(std::unique_ptr<libdnf::OptionBool>(new libdnf::OptionBool(false))));

    auto available = ctx.arg_parser.add_new_named_arg("available");
    available->set_long_name("available");
    available->set_short_description("show only available groups");
    available->set_const_value("true");
    available->link_value(available_option);

    auto installed = ctx.arg_parser.add_new_named_arg("installed");
    installed->set_long_name("installed");
    installed->set_short_description("show only installed groups");
    installed->set_const_value("true");
    installed->link_value(installed_option);

    auto hidden = ctx.arg_parser.add_new_named_arg("hidden");
    hidden->set_long_name("hidden");
    hidden->set_short_description("show also hidden groups");
    hidden->set_const_value("true");
    hidden->link_value(hidden_option);

    patterns_to_show_options = ctx.arg_parser.add_new_values();
    auto keys = ctx.arg_parser.add_new_positional_arg(
        "groups_to_show",
        ArgumentParser::PositionalArg::UNLIMITED,
        ctx.arg_parser.add_init_value(std::unique_ptr<libdnf::Option>(new libdnf::OptionString(nullptr))),
        patterns_to_show_options);
    keys->set_short_description("List of groups to show");

    auto conflict_args =
        ctx.arg_parser.add_conflict_args_group(std::unique_ptr<std::vector<ArgumentParser::Argument *>>(
            new std::vector<ArgumentParser::Argument *>{available, installed}));

    available->set_conflict_arguments(conflict_args);
    installed->set_conflict_arguments(conflict_args);

    auto grouplist = ctx.arg_parser.add_new_command("grouplist");
    grouplist->set_short_description("display groups");
    grouplist->set_description("");
    grouplist->set_named_args_help_header("Optional arguments:");
    grouplist->set_positional_args_help_header("Positional arguments:");
    grouplist->set_parse_hook_func([this, &ctx](
                                [[maybe_unused]] ArgumentParser::Argument * arg,
                                [[maybe_unused]] const char * option,
                                [[maybe_unused]] int argc,
                                [[maybe_unused]] const char * const argv[]) {
        ctx.select_command(this);
        return true;
    });

    grouplist->register_named_arg(available);
    grouplist->register_named_arg(installed);
    grouplist->register_named_arg(hidden);
    grouplist->register_positional_arg(keys);

    ctx.arg_parser.get_root_command()->register_command(grouplist);
}


void CmdGrouplist::run([[maybe_unused]] Context & ctx) {
    std::vector<std::string> patterns_to_show;
    if (patterns_to_show_options->size() > 0) {
        patterns_to_show.reserve(patterns_to_show_options->size());
        for (auto & pattern : *patterns_to_show_options) {
            auto option = dynamic_cast<libdnf::OptionString *>(pattern.get());
            patterns_to_show.emplace_back(option->get_value());
        }
    }

    // Load group sack
    // TODO(pkratoch): use comps from base and real repositories
    std::unique_ptr<libdnf::Base> base = std::make_unique<libdnf::Base>();
    libdnf::comps::Comps comps(*base.get());
    comps.load_installed();
    std::filesystem::path data_path = PROJECT_SOURCE_DIR "/test/libdnf/comps/data/";
    const char * reponame = "repo";
    comps.load_from_file(data_path / "core.xml", reponame);
    comps.load_from_file(data_path / "standard.xml", reponame);

    libdnf::comps::GroupQuery query = comps.get_group_sack().new_query();

    // Filter by patterns if given
    if (patterns_to_show.size() > 0) {
        auto query_names = libdnf::comps::GroupQuery(query);
        query.filter_groupid(patterns_to_show, libdnf::sack::QueryCmp::IGLOB);
        query |= query_names.filter_name(patterns_to_show, libdnf::sack::QueryCmp::IGLOB);
    } else if (not hidden_option->get_value()) {
        // Filter uservisible only if patterns are not given
        query.filter_uservisible(true);
    }

    std::set<libdnf::comps::Group> group_list;

    auto query_installed = libdnf::comps::GroupQuery(query);
    query_installed.filter_installed(true);

    // --installed -> filter installed groups
    if (installed_option->get_value()) {
        group_list = query_installed.list();
    // --available / all
    } else {
        // all -> first add installed groups to the list
        if (!available_option->get_value()) {
            for (auto group: query_installed.list()) {
                group_list.emplace(group);
            }
        }
        // --available / all -> add available not-installed groups into the list
        auto query_available = libdnf::comps::GroupQuery(query);
        query_available.filter_installed(false);
        std::set<std::string> installed_groupids;
        for (auto group: query_installed.list()) {
            installed_groupids.insert(group.get_groupid());
        }
        for (auto group: query_available.list()) {
            if (installed_groupids.find(group.get_groupid()) == installed_groupids.end()) {
                group_list.emplace(group);
            }
        }
    }

    libdnf::cli::output::print_grouplist_table(group_list);
}


}  // namespace microdnf
