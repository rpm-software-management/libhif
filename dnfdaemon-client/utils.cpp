// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "utils.hpp"

#include <unistd.h>

namespace dnfdaemon::client {

bool am_i_root() noexcept {
    return geteuid() == 0;
}

}  // namespace dnfdaemon::client
