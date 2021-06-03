// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "dbus_goal_wrapper.hpp"

namespace dnfdaemon::client {

DbusGoalWrapper::DbusGoalWrapper(std::vector<dnfdaemon::DbusTransactionItem> transaction) {
    for (auto & ti : transaction) {
        libdnf::transaction::TransactionItemAction action =
            static_cast<libdnf::transaction::TransactionItemAction>(std::get<0>(ti));
        dnfdaemon::KeyValueMap keyval = std::get<1>(ti);
        switch (action) {
            case TransactionItemAction::INSTALL: {
                rpm_installs.push_back(DbusPackageWrapper(keyval));
                break;
            }
            case TransactionItemAction::DOWNGRADE: {
                rpm_downgrades.push_back(DbusPackageWrapper(keyval));
                break;
            }
            case TransactionItemAction::OBSOLETE: {
                rpm_obsoletes.push_back(DbusPackageWrapper(keyval));
                break;
            }
            case TransactionItemAction::UPGRADE: {
                rpm_upgrades.push_back(DbusPackageWrapper(keyval));
                break;
            }
            case TransactionItemAction::REMOVE: {
                rpm_removes.push_back(DbusPackageWrapper(keyval));
                break;
            }
            case TransactionItemAction::REINSTALL: {
                rpm_reinstalls.push_back(DbusPackageWrapper(keyval));
                break;
            }
            case TransactionItemAction::DOWNGRADED:
            case TransactionItemAction::OBSOLETED:
            case TransactionItemAction::UPGRADED:
            case TransactionItemAction::REINSTALLED:
            case TransactionItemAction::REASON_CHANGE: {
                // TODO (nsella) Implement case
                break;
            }
        }
    }
}

}  // namespace dnfdaemon::client
