// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "libdnf/logger/log_router.hpp"

namespace libdnf {

std::unique_ptr<Logger> LogRouter::release_logger(size_t index) {
    auto ret = std::move(loggers.at(index));
    loggers.erase(loggers.begin() + static_cast<int>(index));
    return ret;
}

void LogRouter::log(Level level, const std::string & message) noexcept {
    auto now = time(nullptr);
    auto pid = getpid();
    for (auto & logger : loggers) {
        logger->write(now, pid, level, message);
    }
}

void LogRouter::write(time_t time, pid_t pid, Level level, const std::string & message) noexcept {
    for (auto & logger : loggers) {
        logger->write(time, pid, level, message);
    }
}

}  // namespace libdnf
