// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "utils.hpp"

#include <fcntl.h>
#include <libsmartcols/libsmartcols.h>
#include <pwd.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace microdnf {

bool am_i_root() noexcept {
    return geteuid() == 0;
}

static constexpr uid_t INVALID_UID = static_cast<uid_t>(-1);

/// Read the login uid from the "/proc/self/loginuid".
static uid_t read_login_uid_from_proc() noexcept {
    auto in = open("/proc/self/loginuid", O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
    if (in == -1) {
        return INVALID_UID;
    }

    ssize_t len;
    char buf[16];
    do {
        errno = 0;
        len = read(in, buf, sizeof(buf) - 1);
    } while (len < 0 && errno == EINTR);

    close(in);

    if (len <= 0) {
        return INVALID_UID;
    }

    buf[len] = '\0';
    char * endptr;
    errno = 0;
    auto uid = static_cast<uid_t>(strtol(buf, &endptr, 10));
    if (buf == endptr || errno != 0) {
        return INVALID_UID;
    }

    return uid;
}

uid_t get_login_uid() noexcept {
    static uid_t cached_uid = INVALID_UID;
    if (cached_uid == INVALID_UID) {
        cached_uid = read_login_uid_from_proc();
        if (cached_uid == INVALID_UID) {
            cached_uid = getuid();
        }
    }
    return cached_uid;
}

}  // namespace microdnf
