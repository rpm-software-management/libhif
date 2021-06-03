// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_LOGGER_NULL_LOGGER_HPP
#define LIBDNF_LOGGER_NULL_LOGGER_HPP


#include "logger.hpp"

namespace libdnf {


/// NullLogger is an implementation of logging class that discards all incoming logging messages.
/// It can be used in case when no logs are needed.
///
/// @replaces libdnf:utils/logger.hpp:class:NullLogger
class NullLogger : public Logger {
public:
    /// @replaces libdnf:utils/logger.hpp:method:NullLogger.write(int , libdnf::Logger::Level , const std::string & )
    void log(Level /*level*/, const std::string & /*message*/) noexcept override {}

    /// @replaces libdnf:utils/logger.hpp:method:NullLogger.write(int , time_t , pid_t , libdnf::Logger::Level , const std::string & )
    void write(time_t /*time*/, pid_t /*pid*/, Level /*level*/, const std::string & /*message*/) noexcept override {}
};


}  // namespace libdnf

#endif
