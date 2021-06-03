// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef DNFDAEMON_SERVER_SERVICES_REPOCONF_HPP
#define DNFDAEMON_SERVER_SERVICES_REPOCONF_HPP

#include "dnfdaemon-server/dbus.hpp"
#include "dnfdaemon-server/session.hpp"

#include <sdbus-c++/sdbus-c++.h>

#include <string>
#include <vector>

class RepoConf : public IDbusSessionService {
public:
    using IDbusSessionService::IDbusSessionService;
    ~RepoConf() = default;
    void dbus_register();
    void dbus_deregister();

private:
    sdbus::MethodReply list(sdbus::MethodCall && call);
    sdbus::MethodReply get(sdbus::MethodCall && call);
    sdbus::MethodReply enable_disable(sdbus::MethodCall && call, const bool & enable);
    sdbus::MethodReply enable(sdbus::MethodCall && call) { return enable_disable(std::move(call), true); };
    sdbus::MethodReply disable(sdbus::MethodCall && call) { return enable_disable(std::move(call), false); };

    dnfdaemon::KeyValueMapList repo_list(const std::vector<std::string> & ids);
    std::vector<std::string> enable_disable_repos(const std::vector<std::string> & ids, const bool enable);
};

#endif
