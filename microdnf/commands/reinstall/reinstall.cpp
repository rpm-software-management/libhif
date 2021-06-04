// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "reinstall.hpp"

#include "../../context.hpp"

#include "libdnf-cli/output/transaction_table.hpp"

#include <libdnf/base/goal.hpp>
#include <libdnf/conf/option_string.hpp>
#include <libdnf/rpm/package.hpp>
#include <libdnf/rpm/package_query.hpp>
#include <libdnf/rpm/package_set.hpp>
#include <libdnf/rpm/transaction.hpp>

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace microdnf {

using namespace libdnf::cli;

void CmdReinstall::set_argument_parser(Context & ctx) {
    patterns_to_reinstall_options = ctx.arg_parser.add_new_values();
    auto keys = ctx.arg_parser.add_new_positional_arg(
        "keys_to_match",
        ArgumentParser::PositionalArg::UNLIMITED,
        ctx.arg_parser.add_init_value(std::unique_ptr<libdnf::Option>(new libdnf::OptionString(nullptr))),
        patterns_to_reinstall_options);
    keys->set_short_description("List of keys to match");

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
    auto package_sack = ctx.base.get_rpm_package_sack();

    // To search in the system repository (installed packages)
    // Creates system repository in the repo_sack and loads it into rpm::PackageSack.
    package_sack->create_system_repo(false);

    // To search in available repositories (available packages)
    auto enabled_repos = ctx.base.get_rpm_repo_sack()->new_query().filter_enabled(true);
    using LoadFlags = libdnf::rpm::PackageSack::LoadRepoFlags;
    auto flags = LoadFlags::USE_FILELISTS | LoadFlags::USE_PRESTO | LoadFlags::USE_UPDATEINFO | LoadFlags::USE_OTHER;
    ctx.load_rpm_repos(enabled_repos, flags);

    libdnf::Goal goal(&ctx.base);
    for (auto & pattern : *patterns_to_reinstall_options) {
        auto option = dynamic_cast<libdnf::OptionString *>(pattern.get());
        goal.add_rpm_reinstall(option->get_value());
    }
    if (goal.resolve(true) != libdnf::GoalProblem::NO_PROBLEM) {
        std::cout << goal.get_formated_all_problems() << std::endl;
        return;
    }

    if (!libdnf::cli::output::print_transaction_table(goal)) {
        return;
    }

    if (!userconfirm(ctx.base.get_config())) {
        std::cout << "Operation aborted." << std::endl;
        return;
    }

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
