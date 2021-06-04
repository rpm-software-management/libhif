// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef DNFDAEMON_SERVER_SERVICES_REPO_REPO_HPP
#define DNFDAEMON_SERVER_SERVICES_REPO_REPO_HPP

#include "dnfdaemon-server/session.hpp"

#include <sdbus-c++/sdbus-c++.h>

#include <string>
#include <vector>

class Repo : public IDbusSessionService {
public:
    using IDbusSessionService::IDbusSessionService;
    ~Repo() = default;
    void dbus_register();
    void dbus_deregister();

private:
    sdbus::MethodReply list(sdbus::MethodCall && call);
};

#endif
