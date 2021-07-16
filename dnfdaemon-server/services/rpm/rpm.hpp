/*
Copyright (C) 2020 Red Hat, Inc.

This file is part of dnfdaemon-server: https://github.com/rpm-software-management/libdnf/

Dnfdaemon-server is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Dnfdaemon-server is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with dnfdaemon-server.  If not, see <https://www.gnu.org/licenses/>.
*/

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
    sdbus::MethodReply list(sdbus::MethodCall & call);
    sdbus::MethodReply install(sdbus::MethodCall & call);
    sdbus::MethodReply upgrade(sdbus::MethodCall & call);
    sdbus::MethodReply remove(sdbus::MethodCall & call);
    sdbus::MethodReply downgrade(sdbus::MethodCall & call);
};

#endif
