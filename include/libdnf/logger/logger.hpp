// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_LOGGER_LOGGER_HPP
#define LIBDNF_LOGGER_LOGGER_HPP


#include <unistd.h>

#include <array>
#include <ctime>
#include <string>

namespace libdnf {


/// Logger is an abstract interface used for logging.
/// An implementation (inherited class) can call callbacks, log the messages to memory, file, or somewhere else.
///
/// @replaces libdnf:utils/logger.hpp:class:Logger
class Logger {
public:
    Logger() = default;
    Logger(const Logger &) = delete;
    Logger(Logger &&) = delete;
    Logger & operator=(const Logger &) = delete;
    Logger & operator=(Logger &&) = delete;
    virtual ~Logger() = default;

    enum class Level : unsigned int { CRITICAL, ERROR, WARNING, NOTICE, INFO, DEBUG, TRACE };

    static constexpr const char * level_to_cstr(Level level) noexcept {
        auto ilevel = static_cast<unsigned int>(level);
        return ilevel >= LEVEL_C_STR.size() ? "UNDEFINED" : LEVEL_C_STR[ilevel];
    }

    /// @replaces libdnf:utils/logger.hpp:method:Logger.critical(const std::string & message)
    void critical(const std::string & message) noexcept { log(Level::CRITICAL, message); }

    /// @replaces libdnf:utils/logger.hpp:method:Logger.error(const std::string & message)
    void error(const std::string & message) noexcept { log(Level::ERROR, message); }

    /// @replaces libdnf:utils/logger.hpp:method:Logger.warning(const std::string & message)
    void warning(const std::string & message) noexcept { log(Level::WARNING, message); }

    /// @replaces libdnf:utils/logger.hpp:method:Logger.notice(const std::string & message)
    void notice(const std::string & message) noexcept { log(Level::NOTICE, message); }

    /// @replaces libdnf:utils/logger.hpp:method:Logger.info(const std::string & message)
    void info(const std::string & message) noexcept { log(Level::INFO, message); }

    /// @replaces libdnf:utils/logger.hpp:method:Logger.debug(const std::string & message)
    void debug(const std::string & message) noexcept { log(Level::DEBUG, message); }

    /// @replaces libdnf:utils/logger.hpp:method:Logger.trace(const std::string & message)
    void trace(const std::string & message) noexcept { log(Level::TRACE, message); }

    /// @replaces libdnf:utils/logger.hpp:method:Logger.write(libdnf::Logger::Level level, const std::string & message)
    virtual void log(Level level, const std::string & message) noexcept;

    /// @replaces libdnf:utils/logger.hpp:method:Logger.write(time_t time, pid_t pid, libdnf::Logger::Level level, const std::string & message)
    virtual void write(time_t time, pid_t pid, Level level, const std::string & message) noexcept = 0;

private:
#ifndef SWIG
    static constexpr std::array<const char *, static_cast<unsigned int>(Level::TRACE) + 1> LEVEL_C_STR = {
        "CRITICAL", "ERROR", "WARNING", "NOTICE", "INFO", "DEBUG", "TRACE"};
#endif
};


}  // namespace libdnf

#endif
