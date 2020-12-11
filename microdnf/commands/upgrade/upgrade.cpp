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

#include "upgrade.hpp"
#include "../../utils.hpp"

#include "../../context.hpp"

#include <libdnf/base/goal.hpp>
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

void CmdUpgrade::set_argument_parser(Context & ctx) {
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

    auto upgrade = ctx.arg_parser.add_new_command("upgrade");
    upgrade->set_short_description("upgrade a package or packages on your system");
    upgrade->set_description("");
    upgrade->set_named_args_help_header("Optional arguments:");
    upgrade->set_positional_args_help_header("Positional arguments:");
    upgrade->set_parse_hook_func([this, &ctx](
                                [[maybe_unused]] ArgumentParser::Argument * arg,
                                [[maybe_unused]] const char * option,
                                [[maybe_unused]] int argc,
                                [[maybe_unused]] const char * const argv[]) {
        ctx.select_command(this);
        return true;
    });

    upgrade->register_positional_arg(keys);

    ctx.arg_parser.get_root_command()->register_command(upgrade);
}

void CmdUpgrade::configure([[maybe_unused]] Context & ctx) {}

void CmdUpgrade::run(Context & ctx) {
    auto & solv_sack = ctx.base.get_rpm_solv_sack();

    // To search in the system repository (installed packages)
    // Creates system repository in the repo_sack and loads it into rpm::SolvSack.
    solv_sack.create_system_repo(false);

    // To search in available repositories (available packages)
    auto enabled_repos = ctx.base.get_rpm_repo_sack().new_query().ifilter_enabled(true);
    using LoadFlags = libdnf::rpm::SolvSack::LoadRepoFlags;
    auto flags = LoadFlags::USE_FILELISTS | LoadFlags::USE_PRESTO | LoadFlags::USE_UPDATEINFO | LoadFlags::USE_OTHER;
    ctx.load_rpm_repos(enabled_repos, flags);

    // Adds remote packages (defined by a path on the command line) to the sack.
    auto remote_packages = add_remote_packages(ctx, package_paths, false);

    std::cout << std::endl;

    libdnf::Goal goal(&ctx.base);
    for (auto & package : remote_packages) {
        goal.add_rpm_upgrade(package);
    }
    for (auto & pattern : specs) {
        goal.add_rpm_upgrade(pattern, {});
    }
    goal.resolve();

    if (!print_goal(goal)) {
        return;
    }

    if (!userconfirm(ctx.base.get_config())) {
        std::cout << "Operation aborted." << std::endl;
        return;
    }

    download_packages(goal, nullptr);

    std::cout << std::endl;

    libdnf::rpm::Transaction rpm_transaction(ctx.base);
    auto db_transaction = new_db_transaction(ctx);
    std::vector<std::unique_ptr<RpmTransactionItem>> transaction_items;

    fill_transactions(goal, db_transaction, rpm_transaction, transaction_items);

    auto time = std::chrono::system_clock::now().time_since_epoch();
    db_transaction->set_dt_start(std::chrono::duration_cast<std::chrono::seconds>(time).count());
    db_transaction->start();

    run_transaction(rpm_transaction);

    time = std::chrono::system_clock::now().time_since_epoch();
    db_transaction->set_dt_end(std::chrono::duration_cast<std::chrono::seconds>(time).count());
    db_transaction->finish(libdnf::transaction::TransactionState::DONE);
}

}  // namespace microdnf
