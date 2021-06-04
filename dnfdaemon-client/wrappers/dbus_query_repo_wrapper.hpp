// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef DNFDAEMON_CLIENT_WRAPPERS_DBUS_QUERY_REPO_WRAPPER_HPP
#define DNFDAEMON_CLIENT_WRAPPERS_DBUS_QUERY_REPO_WRAPPER_HPP

#include "dbus_repo_wrapper.hpp"

#include <dnfdaemon-server/dbus.hpp>

#include <memory>
#include <vector>

namespace dnfdaemon::client {

class DbusQueryRepoWrapper {
public:
    DbusQueryRepoWrapper(dnfdaemon::KeyValueMapList & repositories);

    const std::vector<std::unique_ptr<DbusRepoWrapper>> & get_data() const { return queryrepo; }

private:
    std::vector<std::unique_ptr<DbusRepoWrapper>> queryrepo;
};

}  // namespace dnfdaemon::client

#endif  // DNFDAEMON_CLIENT_WRAPPERS_DBUS_QUERY_REPO_WRAPPER_HPP
