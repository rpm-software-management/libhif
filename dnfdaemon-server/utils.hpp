// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef DNFDAEMON_SERVER_UTILS_HPP
#define DNFDAEMON_SERVER_UTILS_HPP

#include "dbus.hpp"

#include <sdbus-c++/sdbus-c++.h>

#include <string>


template <typename ItemType>
ItemType key_value_map_get(const dnfdaemon::KeyValueMap & map, const std::string & key) {
    auto it = map.find(key);
    if (it == map.end()) {
        throw sdbus::Error(dnfdaemon::ERROR, "Key not present in the map.");
    }
    try {
        return it->second.get<ItemType>();
    } catch (sdbus::Error & e) {
        throw sdbus::Error(dnfdaemon::ERROR, "Incorrect map item type.");
    }
}

template <typename ItemType>
ItemType key_value_map_get(const dnfdaemon::KeyValueMap & map, const std::string & key, const ItemType & default_value) {
    auto it = map.find(key);
    if (it == map.end()) {
        return default_value;
    }
    try {
        return it->second.get<ItemType>();
    } catch (sdbus::Error & e) {
        throw sdbus::Error(dnfdaemon::ERROR, "Incorrect map item type.");
    }
}

#endif
