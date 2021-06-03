// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef DNFDAEMON_SERVER_SERVICES_RPM_RPM_HPP
#define DNFDAEMON_SERVER_SERVICES_RPM_RPM_HPP

#include "dnfdaemon-server/session.hpp"

#include <sdbus-c++/sdbus-c++.h>

class Rpm : public IDbusSessionService {
public:
    using IDbusSessionService::IDbusSessionService;
    ~Rpm() = default;
    void dbus_register();
    void dbus_deregister();

private:
    sdbus::MethodReply list(sdbus::MethodCall && call);
    sdbus::MethodReply install(sdbus::MethodCall && call);
    sdbus::MethodReply upgrade(sdbus::MethodCall && call);
    sdbus::MethodReply remove(sdbus::MethodCall && call);
    sdbus::MethodReply downgrade(sdbus::MethodCall && call);
};

#endif
