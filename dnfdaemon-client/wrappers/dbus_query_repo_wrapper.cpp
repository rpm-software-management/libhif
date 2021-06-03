// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "dbus_query_repo_wrapper.hpp"

namespace dnfdaemon::client {

DbusQueryRepoWrapper::DbusQueryRepoWrapper(dnfdaemon::KeyValueMapList & repositories) {
    for (auto raw_repo : repositories) {
        queryrepo.push_back(std::make_unique<DbusRepoWrapper>(raw_repo));
    }
}

}  // namespace dnfdaemon::client
