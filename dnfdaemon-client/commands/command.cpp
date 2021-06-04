// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "command.hpp"

#include "../context.hpp"
#include "../utils.hpp"
#include "../wrappers/dbus_package_wrapper.hpp"
#include "../wrappers/dbus_goal_wrapper.hpp"

#include <dnfdaemon-server/dbus.hpp>

#include <iostream>
#include <vector>

#include "libdnf-cli/output/transaction_table.hpp"

namespace dnfdaemon::client {

void TransactionCommand::run_transaction(Context & ctx) {
    dnfdaemon::KeyValueMap options = {};

    // resolve the transaction
    options["allow_erasing"] = ctx.allow_erasing.get_value();
    std::vector<dnfdaemon::DbusTransactionItem> transaction;
    ctx.session_proxy->callMethod("resolve")
        .onInterface(dnfdaemon::INTERFACE_GOAL)
        .withTimeout(static_cast<uint64_t>(-1))
        .withArguments(options)
        .storeResultsTo(transaction);
    if (transaction.empty()) {
        std::cout << "Nothing to do." << std::endl;
        return;
    }

    // print the transaction to the user and ask for confirmation
    DbusGoalWrapper dbus_goal_wrapper(transaction);
    libdnf::cli::output::print_transaction_table(dbus_goal_wrapper);

    if (!userconfirm(ctx)) {
        std::cout << "Operation aborted." << std::endl;
        return;
    }

    // do the transaction
    options.clear();
    ctx.session_proxy->callMethod("do_transaction")
        .onInterface(dnfdaemon::INTERFACE_GOAL)
        .withTimeout(static_cast<uint64_t>(-1))
        .withArguments(options);
}

}  // namespace dnfdaemon::client
