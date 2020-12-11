/*
Copyright (C) 2019-2020 Red Hat, Inc.

This file is part of microdnf: https://github.com/rpm-software-management/libdnf/

Microdnf is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Microdnf is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with microdnf.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "repoquery.hpp"

#include "../../context.hpp"

#include <libdnf/conf/option_string.hpp>
#include <libdnf/rpm/package.hpp>
#include <libdnf/rpm/package_set.hpp>
#include <libdnf/rpm/solv_query.hpp>

#include "libdnf-cli/output/repoquery.hpp"

#include <iostream>

namespace microdnf {

using namespace libdnf::cli;
void CmdRepoquery::set_argument_parser(Context & ctx) {
    available_option = dynamic_cast<libdnf::OptionBool *>(
        ctx.arg_parser.add_init_value(std::unique_ptr<libdnf::OptionBool>(new libdnf::OptionBool(true))));

    installed_option = dynamic_cast<libdnf::OptionBool *>(
        ctx.arg_parser.add_init_value(std::unique_ptr<libdnf::OptionBool>(new libdnf::OptionBool(false))));

    info_option = dynamic_cast<libdnf::OptionBool *>(
        ctx.arg_parser.add_init_value(std::unique_ptr<libdnf::OptionBool>(new libdnf::OptionBool(false))));

    nevra_option = dynamic_cast<libdnf::OptionBool *>(
        ctx.arg_parser.add_init_value(std::unique_ptr<libdnf::OptionBool>(new libdnf::OptionBool(true))));

    auto available = ctx.arg_parser.add_new_named_arg("available");
    available->set_long_name("available");
    available->set_short_description("display available packages (default)");
    available->set_const_value("true");
    available->link_value(available_option);

    auto installed = ctx.arg_parser.add_new_named_arg("installed");
    installed->set_long_name("installed");
    installed->set_short_description("display installed packages");
    installed->set_const_value("true");
    installed->link_value(installed_option);

    auto info = ctx.arg_parser.add_new_named_arg("info");
    info->set_long_name("info");
    info->set_short_description("show detailed information about the packages");
    info->set_const_value("true");
    info->link_value(info_option);

    auto nevra = ctx.arg_parser.add_new_named_arg("nevra");
    nevra->set_long_name("nevra");
    nevra->set_short_description(
        "use name-epoch:version-release.architecture format for displaying packages (default)");
    nevra->set_const_value("true");
    nevra->link_value(nevra_option);

    auto keys = ctx.arg_parser.add_new_positional_arg(
        "keys_to_match",
        ArgumentParser::PositionalArg::UNLIMITED,
        nullptr,
        nullptr);
    keys->set_short_description("List of keys to match");
    keys->set_parse_hook_func([&](ArgumentParser::PositionalArg *, int argc, const char * const argv[]){
        for (int i = 0; i < argc; ++i) {
            switch (get_key_type(argv[i])) {
                case KeyType::PACKAGE_FILE:
                    package_paths.insert(argv[i]);
                    break;
                default:
                    specs.insert(argv[i]);
                    break;
            }
        }
        return true;
    });

    auto conflict_args =
        ctx.arg_parser.add_conflict_args_group(std::unique_ptr<std::vector<ArgumentParser::Argument *>>(
            new std::vector<ArgumentParser::Argument *>{info, nevra}));

    info->set_conflict_arguments(conflict_args);
    nevra->set_conflict_arguments(conflict_args);

    auto repoquery = ctx.arg_parser.add_new_command("repoquery");
    repoquery->set_short_description("search for packages matching keyword");
    repoquery->set_description("");
    repoquery->set_named_args_help_header("Optional arguments:");
    repoquery->set_positional_args_help_header("Positional arguments:");
    repoquery->set_parse_hook_func([this, &ctx](
                                [[maybe_unused]] ArgumentParser::Argument * arg,
                                [[maybe_unused]] const char * option,
                                [[maybe_unused]] int argc,
                                [[maybe_unused]] const char * const argv[]) {
        ctx.select_command(this);
        return true;
    });

    repoquery->register_named_arg(available);
    repoquery->register_named_arg(installed);
    repoquery->register_named_arg(info);
    repoquery->register_named_arg(nevra);
    repoquery->register_positional_arg(keys);

    ctx.arg_parser.get_root_command()->register_command(repoquery);
}

void CmdRepoquery::configure([[maybe_unused]] Context & ctx) {}

void CmdRepoquery::run(Context & ctx) {
    auto & solv_sack = ctx.base.get_rpm_solv_sack();
    std::vector<libdnf::rpm::Package> remote_packages;

    // To search in the system repository (installed packages)
    if (installed_option->get_value()) {
        // Creates system repository in the repo_sack and loads it into rpm::SolvSack.
        solv_sack.create_system_repo(false);
    }

    // To search in available repositories (available packages)
    if (available_option->get_priority() >= libdnf::Option::Priority::COMMANDLINE || !installed_option->get_value()) {
        auto enabled_repos = ctx.base.get_rpm_repo_sack().new_query().ifilter_enabled(true);
        using LoadFlags = libdnf::rpm::SolvSack::LoadRepoFlags;
        auto flags =
            LoadFlags::USE_FILELISTS | LoadFlags::USE_PRESTO | LoadFlags::USE_UPDATEINFO | LoadFlags::USE_OTHER;
        ctx.load_rpm_repos(enabled_repos, flags);
        std::cout << std::endl;

        // Adds remote packages (defined by a path on the command line) to the sack.
        remote_packages = add_remote_packages(ctx, package_paths, false);
    }

    libdnf::rpm::PackageSet result_pset(&solv_sack);
    for (auto & package : remote_packages) {
        result_pset.add(package);
    }
    libdnf::rpm::SolvQuery full_solv_query(&solv_sack);
    for (auto & pattern : specs) {
        libdnf::rpm::SolvQuery solv_query(full_solv_query);
        solv_query.resolve_pkg_spec(pattern, true, true, true, true, true, {});
        result_pset |= solv_query.get_package_set();
    }

    if (info_option->get_value()) {
        for (auto package : result_pset) {
            libdnf::cli::output::print_package_info_table(package);
            std::cout << '\n';
        }
    } else {
        for (auto package : result_pset) {
            std::cout << package.get_full_nevra() << '\n';
        }
    }
}

}  // namespace microdnf
