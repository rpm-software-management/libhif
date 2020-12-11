/*
Copyright (C) 2020 Red Hat, Inc.

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

#include "reinstall.hpp"

#include "../../context.hpp"

#include <libdnf/conf/option_string.hpp>
#include <libdnf/rpm/package.hpp>
#include <libdnf/rpm/package_set.hpp>
#include <libdnf/rpm/solv_query.hpp>
#include <libdnf/rpm/transaction.hpp>

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

namespace microdnf {

using namespace libdnf::cli;

void CmdReinstall::set_argument_parser(Context & ctx) {
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

    auto reinstall = ctx.arg_parser.add_new_command("reinstall");
    reinstall->set_short_description("reinstall a package or packages");
    reinstall->set_description("");
    reinstall->set_named_args_help_header("Optional arguments:");
    reinstall->set_positional_args_help_header("Positional arguments:");
    reinstall->set_parse_hook_func([this, &ctx](
                                [[maybe_unused]] ArgumentParser::Argument * arg,
                                [[maybe_unused]] const char * option,
                                [[maybe_unused]] int argc,
                                [[maybe_unused]] const char * const argv[]) {
        ctx.select_command(this);
        return true;
    });

    reinstall->register_positional_arg(keys);

    ctx.arg_parser.get_root_command()->register_command(reinstall);
}

void CmdReinstall::configure([[maybe_unused]] Context & ctx) {}

void CmdReinstall::run(Context & ctx) {
    auto & solv_sack = ctx.base.get_rpm_solv_sack();

    // To search in the system repository (installed packages)
    // Creates system repository in the repo_sack and loads it into rpm::SolvSack.
    //solv_sack.create_system_repo(false);

    // To search in available repositories (available packages)
    auto enabled_repos = ctx.base.get_rpm_repo_sack().new_query().ifilter_enabled(true);
    using LoadFlags = libdnf::rpm::SolvSack::LoadRepoFlags;
    auto flags = LoadFlags::USE_FILELISTS | LoadFlags::USE_PRESTO | LoadFlags::USE_UPDATEINFO | LoadFlags::USE_OTHER;
    ctx.load_rpm_repos(enabled_repos, flags);

    // Adds remote packages (defined by a path on the command line) to the sack.
    auto remote_packages = add_remote_packages(ctx, package_paths, false);

    std::cout << std::endl;

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

    std::vector<libdnf::rpm::Package> download_pkgs;
    download_pkgs.reserve(result_pset.size());
    for (auto package : result_pset) {
        if (package.get_repo()->get_id() != "@commandline") {
            download_pkgs.push_back(std::move(package));
        }
    }
    download_packages(download_pkgs, nullptr);

    std::vector<std::unique_ptr<RpmTransactionItem>> transaction_items;
    auto ts = libdnf::rpm::Transaction(ctx.base);
    for (auto package : result_pset) {
        try {
            auto item = std::make_unique<RpmTransactionItem>(package, RpmTransactionItem::Actions::REINSTALL);
            auto item_ptr = item.get();
            transaction_items.push_back(std::move(item));
            ts.reinstall(*item_ptr);
        } catch (std::exception & ex) {
            std::cerr << ex.what() << std::endl;
        }
    }
    std::cout << std::endl;
    run_transaction(ts);
}

}  // namespace microdnf
