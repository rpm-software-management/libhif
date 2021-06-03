// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "libdnf/logger/memory_buffer_logger.hpp"

namespace libdnf {

MemoryBufferLogger::MemoryBufferLogger(std::size_t max_items_to_keep, std::size_t reserve)
    : max_items(max_items_to_keep)
    , first_item_idx(0) {
    if (reserve > 0) {
        items.reserve(reserve < max_items_to_keep ? reserve : max_items_to_keep);
    }
}

void MemoryBufferLogger::write(time_t time, pid_t pid, Level level, const std::string & message) noexcept {
    try {
        std::lock_guard<std::mutex> guard(items_mutex);
        if (max_items == 0 || items.size() < max_items) {
            items.push_back({time, pid, level, message});
        } else {
            items[first_item_idx] = {time, pid, level, message};
            if (++first_item_idx >= max_items) {
                first_item_idx = 0;
            }
        }
    } catch (...) {
    }
}

const MemoryBufferLogger::Item & MemoryBufferLogger::get_item(std::size_t item_idx) const {
    if (item_idx >= items.size() || item_idx >= max_items) {
        throw std::out_of_range("MemoryBufferLogger");
    }

    std::lock_guard<std::mutex> guard(items_mutex);
    auto idx = item_idx + first_item_idx;
    if (idx >= max_items) {
        idx -= max_items;
    }
    return items[idx];
}

void MemoryBufferLogger::write_to_logger(Logger & logger) {
    std::lock_guard<std::mutex> guard(items_mutex);
    for (size_t idx = first_item_idx; idx < items.size(); ++idx) {
        auto & msg = items[idx];
        logger.write(msg.time, msg.pid, msg.level, msg.message);
    }
    for (size_t idx = 0; idx < first_item_idx; ++idx) {
        auto & msg = items[idx];
        logger.write(msg.time, msg.pid, msg.level, msg.message);
    }
}

void MemoryBufferLogger::clear() noexcept {
    std::lock_guard<std::mutex> guard(items_mutex);
    first_item_idx = 0;
    items.clear();
}

}  // namespace libdnf
