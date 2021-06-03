// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_UTILS_XDG_HPP
#define LIBDNF_UTILS_XDG_HPP

#include <filesystem>

namespace libdnf::utils::xdg {

/// Returns user home directory.
std::filesystem::path get_user_home_dir();

/// Returns user cache directory.
/// A base directory relative to which user specific non-essential data files should be stored.
std::filesystem::path get_user_cache_dir();

/// Returns user configuration directory.
/// A base directory relative to which user specific configuration files should be stored.
std::filesystem::path get_user_config_dir();

/// Returns user data directory.
/// A base directory relative to which user specific data files should be stored.
std::filesystem::path get_user_data_dir();

/// Returns user runtime directory.
/// A base directory relative to which user-specific non-essential runtime files and other file objects
/// (such as sockets, named pipes, ...) should be stored.
std::filesystem::path get_user_runtime_dir();

}  // namespace libdnf::utils::xdg

#endif
