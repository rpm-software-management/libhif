// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_RPM_SOLV_MAP_HPP
#define LIBDNF_RPM_SOLV_MAP_HPP


#include "map_iterator.hpp"

#include <solv/bitmap.h>
#include <solv/pooltypes.h>

namespace libdnf::rpm {

class PackageSack;

}  // namespace libdnf::rpm

namespace libdnf::rpm::solv {


// clang-format off
// see http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetTable
static const unsigned char BIT_COUNT_LOOKUP[256] = {
    #define B2(n) n, n + 1, n + 1, n + 2
    #define B4(n) B2(n), B2(n + 1), B2(n + 1), B2(n + 2)
    #define B6(n) B4(n), B4(n + 1), B4(n + 1), B4(n + 2)
    B6(0), B6(1), B6(1), B6(2)
};
// clang-format on


class SolvMap {
public:
    using iterator = SolvMapIterator;
    iterator begin() const;
    iterator end() const;

    /// Initialize an empty Map of given size
    explicit SolvMap(int size);

    /// Clone from an existing Map
    explicit SolvMap(const Map * map);

    /// Copy constructor: clone from an existing SolvMap
    SolvMap(const SolvMap & other);

    /// Move constructor
    SolvMap(SolvMap && other) noexcept;

    ~SolvMap();

    SolvMap & operator=(const SolvMap & other) noexcept;
    SolvMap & operator=(SolvMap && other) noexcept;

    // GENERIC OPERATIONS

    void grow(int size) { map_grow(&map, size); };

    void set_all() { map_setall(&map); };

    /// Return the underlying libsolv Map
    ///
    /// @replaces libdnf:sack/packageset.hpp:method:PackageSet.getMap()
    const Map * get_map() const noexcept;

    /// Return the number of solvables in the SolvMap (number of 1s in the bitmap).
    ///
    /// @replaces libdnf:sack/packageset.hpp:method:PackageSet.size()
    std::size_t size() const noexcept;

    bool empty() const noexcept;

    void clear() noexcept;

    // ITEM OPERATIONS

    /// @replaces libdnf:sack/packageset.hpp:method:PackageSet.set(Id id)
    void add(Id id);

    /// Faster, but unsafe version of add() method that is doesn't check bitmap range
    void add_unsafe(Id id) noexcept;

    /// @replaces libdnf:sack/packageset.hpp:method:PackageSet.has(Id id)
    bool contains(Id id) const noexcept;

    /// Faster, but unsafe version of contains() method that is doesn't check bitmap range
    bool contains_unsafe(Id id) const noexcept;

    /// @replaces libdnf:sack/packageset.hpp:method:PackageSet.remove(Id id)
    void remove(Id id);

    /// Faster, but unsafe version of remove() method that is doesn't check bitmap range
    void remove_unsafe(Id id) noexcept;

    // SET OPERATIONS - Map

    /// Union operator
    ///
    /// @replaces libdnf:sack/packageset.hpp:method:PackageSet.operator+=(const Map * other)
    SolvMap & operator|=(const Map * other) noexcept;

    /// Difference operator
    ///
    /// @replaces libdnf:sack/packageset.hpp:method:PackageSet.operator-=(const Map * other)
    SolvMap & operator-=(const Map * other) noexcept;

    /// Intersection operator
    ///
    /// @replaces libdnf:sack/packageset.hpp:method:PackageSet.operator/=(const Map * other)
    SolvMap & operator&=(const Map * other) noexcept;

    // SET OPERATIONS - SolvMap

    /// Union operator
    SolvMap & operator|=(const SolvMap & other) noexcept;

    /// Difference operator
    SolvMap & operator-=(const SolvMap & other) noexcept;

    /// Intersection operator
    SolvMap & operator&=(const SolvMap & other) noexcept;

    void swap(SolvMap & other) noexcept;

protected:
    /// Check if `id` is in bitmap range.
    /// Throws std::out_of_range
    void check_id_in_bitmap_range(Id id) const;

private:
    friend class rpm::PackageSack;
    Map map;
};


inline SolvMap::SolvMap(int size) {
    map_init(&map, size);
}


inline SolvMap::SolvMap(const Map * map) {
    map_init_clone(&this->map, map);
}

inline SolvMap::SolvMap(const SolvMap & other) : SolvMap(other.get_map()) {}

inline SolvMap::SolvMap(SolvMap && other) noexcept {
    map = other.map;
    other.map.map = nullptr;
    other.map.size = 0;
}

inline SolvMap::~SolvMap() {
    map_free(&map);
}

inline SolvMap & SolvMap::operator=(const SolvMap & other) noexcept {
    if (this != &other) {
        if (map.size == other.map.size) {
            memcpy(map.map, other.map.map, static_cast<size_t>(map.size));
        } else {
            map_free(&map);
            map_init_clone(&map, &other.map);
        }
    }
    return *this;
}

inline SolvMap & SolvMap::operator=(SolvMap && other) noexcept {
    if (this != &other) {
        map = other.map;
        other.map.map = nullptr;
        other.map.size = 0;
    }
    return *this;
}

inline void SolvMap::check_id_in_bitmap_range(Id id) const {
    // map.size is in bytes, << 3 multiplies the number with 8 and gives size in bits
    if (id < 0 || id >= (map.size << 3)) {
        throw std::out_of_range("Id is out of bitmap range");
    }
}


inline SolvMap::iterator SolvMap::begin() const {
    iterator it(&map);
    return it;
}


inline SolvMap::iterator SolvMap::end() const {
    iterator it(&map);
    it.end();
    return it;
}


inline void SolvMap::add(Id id) {
    check_id_in_bitmap_range(id);
    add_unsafe(id);
}


inline bool SolvMap::contains(Id id) const noexcept {
    if (id < 0 || id >= (map.size << 3)) {
        // if Id is outside bitmap range, then bitmap doesn't contain it
        return false;
    }
    return contains_unsafe(id);
}


inline void SolvMap::remove(Id id) {
    check_id_in_bitmap_range(id);
    remove_unsafe(id);
}


inline void SolvMap::add_unsafe(Id id) noexcept {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
    MAPSET(&map, id);
#pragma GCC diagnostic pop
}


inline bool SolvMap::contains_unsafe(Id id) const noexcept {
    return MAPTST(&map, id);
}


inline void SolvMap::remove_unsafe(Id id) noexcept {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
    MAPCLR(&map, id);
#pragma GCC diagnostic pop
}


inline const Map * SolvMap::get_map() const noexcept {
    return &map;
}


inline bool SolvMap::empty() const noexcept {
    const unsigned char * byte = map.map;
    const unsigned char * end = byte + map.size;

    // iterate through the whole bitmap by moving the address
    while (byte < end) {
        if (*byte++) {
            // return false if a non-zero bit was found
            return false;
        }
    }
    // all bits were zero, return true
    return true;
}


inline std::size_t SolvMap::size() const noexcept {
    unsigned char * byte = map.map;
    unsigned char * end = byte + map.size;
    std::size_t result = 0;

    // iterate through the whole bitmap by moving the address
    while (byte < end) {
        // add number of bits in each byte
        result += BIT_COUNT_LOOKUP[*byte++];
    }
    return result;
}


inline void SolvMap::clear() noexcept {
    map_empty(&map);
}


inline SolvMap & SolvMap::operator|=(const Map * other) noexcept {
    map_or(&map, const_cast<Map *>(other));
    return *this;
}


inline SolvMap & SolvMap::operator|=(const SolvMap & other) noexcept {
    *this |= other.get_map();
    return *this;
}


inline SolvMap & SolvMap::operator-=(const Map * other) noexcept {
    map_subtract(&map, const_cast<Map *>(other));
    return *this;
}


inline SolvMap & SolvMap::operator-=(const SolvMap & other) noexcept {
    *this -= other.get_map();
    return *this;
}


inline SolvMap & SolvMap::operator&=(const Map * other) noexcept {
    map_and(&map, other);
    return *this;
}


inline SolvMap & SolvMap::operator&=(const SolvMap & other) noexcept {
    *this &= other.get_map();
    return *this;
}

inline void SolvMap::swap(SolvMap & other) noexcept {
    std::swap(map, other.map);
}


}  // namespace libdnf::rpm::solv


#endif  // LIBDNF_RPM_SOLV_MAP_HPP
