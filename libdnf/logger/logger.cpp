// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "libdnf/logger/logger.hpp"

namespace libdnf {

void Logger::log(Level level, const std::string & message) noexcept {
    write(time(nullptr), getpid(), level, message);
}

}  // namespace libdnf
