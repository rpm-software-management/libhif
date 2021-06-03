// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef DNFDAEMON_SERVER_SERVICES_BASE_BASE_HPP
#define DNFDAEMON_SERVER_SERVICES_BASE_BASE_HPP

#include "dnfdaemon-server/session.hpp"

#include <sdbus-c++/sdbus-c++.h>

#include <string>
#include <vector>

class Base : public IDbusSessionService {
public:
    using IDbusSessionService::IDbusSessionService;
    ~Base() = default;
    void dbus_register();
    void dbus_deregister();

private:
    sdbus::MethodReply read_all_repos(sdbus::MethodCall && call);
};

#endif
