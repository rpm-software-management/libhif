// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef DNFDAEMON_SERVER_SERVICES_GOAL_GOAL_HPP
#define DNFDAEMON_SERVER_SERVICES_GOAL_GOAL_HPP

#include "dnfdaemon-server/session.hpp"

#include <sdbus-c++/sdbus-c++.h>

class Goal : public IDbusSessionService {
public:
    using IDbusSessionService::IDbusSessionService;
    ~Goal() = default;
    void dbus_register();
    void dbus_deregister();

private:
    sdbus::MethodReply resolve(sdbus::MethodCall && call);
    sdbus::MethodReply do_transaction(sdbus::MethodCall && call);
};

#endif
