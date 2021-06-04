// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef MICRODNF_UTILS_HPP
#define MICRODNF_UTILS_HPP

#include <libdnf/base/goal.hpp>
#include <sys/types.h>

#include <string>

namespace microdnf {

/// Returns "true" if program runs with effective user ID = 0
bool am_i_root() noexcept;

/// Gets the login uid, if available.
/// The getuid() is returned instead if there was a problem.
/// The value is cached.
uid_t get_login_uid() noexcept;

/// find the base architecture
const char * get_base_arch(const char * arch);

/// detect hardware architecture
std::string detect_arch();

/// detect operation system release
std::string detect_release(const std::string & install_root_path);

}  // namespace microdnf

#endif
