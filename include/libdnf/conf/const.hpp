// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_CONF_CONST_HPP
#define LIBDNF_CONF_CONST_HPP

#include <string>
#include <vector>

namespace libdnf {

constexpr const char * PERSISTDIR = "/var/lib/dnf";
constexpr const char * SYSTEM_CACHEDIR = "/var/cache/dnf";

constexpr const char * CONF_FILENAME = "/etc/dnf/dnf.conf";
constexpr const char * CONF_DIRECTORY = "/etc/dnf/conf.d";

// More important varsdirs must be on the end of vector
const std::vector<std::string> VARS_DIRS{"/etc/yum/vars", "/etc/dnf/vars"};

const std::vector<std::string> GROUP_PACKAGE_TYPES{"mandatory", "default", "conditional"};
const std::vector<std::string> INSTALLONLYPKGS{"kernel",
                                               "kernel-PAE",
                                               "installonlypkg(kernel)",
                                               "installonlypkg(kernel-module)",
                                               "installonlypkg(vm)",
                                               "multiversion(kernel)"};

constexpr const char * BUGTRACKER = "https://bugzilla.redhat.com/enter_bug.cgi?product=Fedora&component=dnf";

}  // namespace libdnf

#endif
