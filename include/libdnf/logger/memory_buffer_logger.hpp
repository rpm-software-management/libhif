// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_LOGGER_MEMORY_BUFFER_LOGGER_HPP
#define LIBDNF_LOGGER_MEMORY_BUFFER_LOGGER_HPP


#include "logger.hpp"

#include <mutex>
#include <vector>

namespace libdnf {


/// MemoryBufferLogger is an implementation of logging class that stores incoming logging messages into memory buffer.
/// It is usually used as temporary logger until a final logger is not configured.
class MemoryBufferLogger : public Logger {
public:
    struct Item {
        time_t time;
        pid_t pid;
        Level level;
        std::string message;
    };

    explicit MemoryBufferLogger(std::size_t max_items_to_keep, std::size_t reserve = 0);
    void write(time_t time, pid_t pid, Level level, const std::string & message) noexcept override;
    std::size_t get_items_count() const { return items.size(); }
    const Item & get_item(std::size_t item_idx) const;
    void clear() noexcept;
    void write_to_logger(Logger & logger);

private:
    mutable std::mutex items_mutex;
    std::size_t max_items;  // rotation, oldest messages are replaced
    std::size_t first_item_idx;
    std::vector<Item> items;
};


}  // namespace libdnf

#endif
