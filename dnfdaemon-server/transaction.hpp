// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef DNFDAEMON_SERVER_SERVICES_RPM_TRANSACTION_HPP
#define DNFDAEMON_SERVER_SERVICES_RPM_TRANSACTION_HPP

#include <libdnf/rpm/package.hpp>
#include <libdnf/rpm/transaction.hpp>
#include <sdbus-c++/sdbus-c++.h>

#include <string>

namespace dnfdaemon {

class RpmTransactionItem : public libdnf::rpm::TransactionItem {
public:
    enum class Actions { INSTALL, ERASE, UPGRADE, DOWNGRADE, REINSTALL, CLEANUP };

    RpmTransactionItem(libdnf::rpm::Package pkg, Actions action) : TransactionItem(pkg), action(action) {}
    Actions get_action() const noexcept { return action; }

private:
    Actions action;
};

}  // namespace dnfdaemon

#endif
