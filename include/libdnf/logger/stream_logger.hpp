// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_LOGGER_STREAM_LOGGER_HPP
#define LIBDNF_LOGGER_STREAM_LOGGER_HPP


#include "logger.hpp"

#include <memory>
#include <mutex>
#include <ostream>

namespace libdnf {


/// StreamLogger is an implementation of logging class that writes messages into a stream.
class StreamLogger : public Logger {
public:
    explicit StreamLogger(std::unique_ptr<std::ostream> && log_stream) : log_stream(std::move(log_stream)) {}
    void write(time_t time, pid_t pid, Level level, const std::string & message) noexcept override;

private:
    mutable std::mutex stream_mutex;
    std::unique_ptr<std::ostream> log_stream;
};


}  // namespace libdnf

#endif
