// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef DNFDAEMON_SERVER_PACKAGE_HPP
#define DNFDAEMON_SERVER_PACKAGE_HPP

#include "dbus.hpp"

#include <libdnf/rpm/package.hpp>

#include <string>
#include <vector>
#include <map>

// TODO(mblaha): add all other package attributes
// package attributes available to be retrieved
enum class PackageAttribute {
    name,
    epoch,
    version,
    release,
    arch,
    repo,
    is_installed,
    install_size,
    package_size,

    nevra,
    full_nevra
};

dnfdaemon::KeyValueMap package_to_map(
    const libdnf::rpm::Package & libdnf_package, const std::vector<std::string> & attributes);

#endif
