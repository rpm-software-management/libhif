// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_RPM_SOLV_MAP_ITERATOR_HPP
#define LIBDNF_RPM_SOLV_MAP_ITERATOR_HPP

#include "libdnf/rpm/package_sack.hpp"

#include <bits/stdc++.h>
#include <solv/bitmap.h>
#include <solv/pooltypes.h>

#include <cstddef>
#include <iterator>


namespace libdnf::rpm::solv {


class SolvMap;


class SolvMapIterator {
public:
    explicit SolvMapIterator(const Map * map) noexcept;
    SolvMapIterator(const SolvMapIterator & other) noexcept = default;

    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = Id;
    using pointer = void;
    using reference = Id;

    Id operator*() const noexcept { return current_value; }

    SolvMapIterator & operator++() noexcept;
    SolvMapIterator operator++(int) noexcept;

    bool operator==(const SolvMapIterator & other) const noexcept { return current_value == other.current_value; }
    bool operator!=(const SolvMapIterator & other) const noexcept { return current_value != other.current_value; }

    void begin() noexcept;
    void end() noexcept;

    /// Sets iterator to the first contained package in the range <id, end>.
    void jump(Id id) noexcept;

protected:
    const Map * get_map() const noexcept { return map; }

private:
    constexpr static int BEGIN = -1;
    constexpr static int END = -2;

    // pointer to a map owned by SolvMap
    const Map * map;

    // current address in the map
    const unsigned char * map_current;

    // the last address in the map
    const unsigned char * map_end;

    // value of the iterator
    Id current_value;
};


inline SolvMapIterator::SolvMapIterator(const Map * map) noexcept : map{map}, map_end{map->map + map->size} {
    begin();
}

inline void SolvMapIterator::begin() noexcept {
    current_value = BEGIN;
    map_current = map->map;
    ++*this;
}

inline void SolvMapIterator::end() noexcept {
    current_value = END;
    map_current = map_end;
}

inline SolvMapIterator & SolvMapIterator::operator++() noexcept {
    if (current_value >= 0) {
        // skip (previous / 8) bytes
        //current += previous >> 3;

        // make a copy of byte with the previous match to avoid changing the map
        unsigned char byte = *map_current;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
        // reset previously seen bits to 0
        byte >>= (current_value & 7) + 1;
#pragma GCC diagnostic pop

        auto bit = ffs(byte << ((current_value & 7) + 1));
        if (bit) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
            // return (current byte * 8) + bit position - 1
            current_value = ((map_current - map->map) << 3) + bit - 1;
#pragma GCC diagnostic pop
            return *this;
        }

        // no following bit was set in the current byte
        // move to the next byte and start searching in the next bytes
        map_current++;
    }

    while (map_current < map_end) {
        // skip all empty bytes
        if (!*map_current) {
            // move to the next byte
            map_current++;
            continue;
        }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
        // now we have a byte that has at least one bit set
        // return (current byte * 8) + bit position - 1
        current_value = ((map_current - map->map) << 3) + ffs(*map_current) - 1;
#pragma GCC diagnostic pop
        return *this;
    }

    // not found
    current_value = END;
    return *this;
}

inline SolvMapIterator SolvMapIterator::operator++(int) noexcept {
    SolvMapIterator it(*this);
    ++*this;
    return it;
}

inline void SolvMapIterator::jump(Id id) noexcept {
    if (id < 0) {
        begin();
        return;
    }

    const unsigned char * current = map->map + (id >> 3);

    if (current >= map_end) {
        end();
        return;
    }

    current_value = id;
    map_current = current;

    // If the element with requested id does not exist in the map, it moves to the next.
    if (!(*current & (1 << (id & 7)))) {
        ++*this;
    }
}


}  // namespace libdnf::rpm::solv


#endif  // LIBDNF_RPM_SOLV_MAP_ITERATOR_HPP
