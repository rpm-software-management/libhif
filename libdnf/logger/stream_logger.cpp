// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "libdnf/logger/stream_logger.hpp"

#include <iomanip>
#include <sstream>

namespace libdnf {

void StreamLogger::write(time_t time, pid_t pid, Level level, const std::string & message) noexcept {
    try {
        struct tm now;

        // gmtime_r() is used because it is thread-safe (std::gmtime() is not).
        gmtime_r(&time, &now);

        std::ostringstream ss;
        ss << std::put_time(&now, "%FT%TZ [");  // "YYYY-MM-DDTHH:MM:SSZ ["
        ss << pid << "] ";
        ss << level_to_cstr(level) << " " << message << "\n";
        std::lock_guard<std::mutex> guard(stream_mutex);
        *log_stream << ss.str() << std::flush;
    } catch (...) {
    }
}

}  // namespace libdnf
