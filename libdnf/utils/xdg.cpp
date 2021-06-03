// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "xdg.hpp"

#include "libdnf/common/exception.hpp"

#include <pwd.h>
#include <unistd.h>

#include <cstdlib>

namespace libdnf::utils::xdg {

std::filesystem::path get_user_home_dir() {
    const char * dir = std::getenv("HOME");
    if (dir && *dir != '\0') {
        auto ret = std::filesystem::path(dir);
        if (ret.is_absolute()) {
            return ret;
        }
    }
    if (struct passwd * pw = getpwuid(getuid())) {
        return std::filesystem::path(pw->pw_dir);
    }
    throw RuntimeError("get_home_dir(): Can't determine the user's home directory");
}

std::filesystem::path get_user_cache_dir() {
    const char * dir = std::getenv("XDG_CACHE_HOME");
    if (dir && *dir != '\0') {
        auto ret = std::filesystem::path(dir);
        if (ret.is_absolute()) {
            return ret;
        }
    }
    return std::filesystem::path(get_user_home_dir()) / ".cache";
}

std::filesystem::path get_user_config_dir() {
    const char * dir = std::getenv("XDG_CONFIG_HOME");
    if (dir && *dir != '\0') {
        auto ret = std::filesystem::path(dir);
        if (ret.is_absolute()) {
            return ret;
        }
    }
    return std::filesystem::path(get_user_home_dir()) / ".config";
}

std::filesystem::path get_user_data_dir() {
    const char * dir = std::getenv("XDG_DATA_HOME");
    if (dir && *dir != '\0') {
        auto ret = std::filesystem::path(dir);
        if (ret.is_absolute()) {
            return ret;
        }
    }
    return std::filesystem::path(get_user_home_dir()) / ".local" / "share";
}

std::filesystem::path get_user_runtime_dir() {
    const char * dir = std::getenv("XDG_RUNTIME_DIR");
    if (dir && *dir != '\0') {
        auto ret = std::filesystem::path(dir);
        if (ret.is_absolute()) {
            return ret;
        }
    }
    throw RuntimeError("get_user_runtime_dir(): Can't determine the user's runtime directory");
}

}  // namespace libdnf::utils::xdg
