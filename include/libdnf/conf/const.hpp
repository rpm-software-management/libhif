/*
Copyright (C) 2018-2020 Red Hat, Inc.

This file is part of libdnf: https://github.com/rpm-software-management/libdnf/

Libdnf is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Libdnf is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with libdnf.  If not, see <https://www.gnu.org/licenses/>.
*/

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
