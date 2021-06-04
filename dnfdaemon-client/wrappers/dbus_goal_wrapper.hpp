// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef DNFDAEMON_CLIENT_WRAPPERS_DBUS_GOAL_WRAPPER_HPP
#define DNFDAEMON_CLIENT_WRAPPERS_DBUS_GOAL_WRAPPER_HPP

#include "dbus_package_wrapper.hpp"

#include <libdnf/transaction/transaction_item_action.hpp>

#include <vector>

using namespace libdnf::transaction;

namespace dnfdaemon::client {

class DbusGoalWrapper {
public:
    DbusGoalWrapper(std::vector<dnfdaemon::DbusTransactionItem>);

    std::vector<DbusPackageWrapper> list_rpm_installs() const { return rpm_installs; };
    std::vector<DbusPackageWrapper> list_rpm_reinstalls() const { return rpm_reinstalls; };
    std::vector<DbusPackageWrapper> list_rpm_upgrades() const { return rpm_upgrades; };
    std::vector<DbusPackageWrapper> list_rpm_downgrades() const { return rpm_downgrades; };
    std::vector<DbusPackageWrapper> list_rpm_removes() const { return rpm_removes; };
    std::vector<DbusPackageWrapper> list_rpm_obsoleted() const { return rpm_obsoletes; };

private:
    std::vector<DbusPackageWrapper> rpm_installs;
    std::vector<DbusPackageWrapper> rpm_reinstalls;
    std::vector<DbusPackageWrapper> rpm_upgrades;
    std::vector<DbusPackageWrapper> rpm_downgrades;
    std::vector<DbusPackageWrapper> rpm_removes;
    std::vector<DbusPackageWrapper> rpm_obsoletes;
};

}  // namespace dnfdaemon::client

#endif  // DNFDAEMON_CLIENT_WRAPEPRS_DBUS_GOAL_WRAPPER_HPP
