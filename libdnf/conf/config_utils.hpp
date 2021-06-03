// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_CONF_CONFIG_PRIVATE_HPP
#define LIBDNF_CONF_CONFIG_PRIVATE_HPP

#include "libdnf/conf/option.hpp"

namespace libdnf {

template <typename T>
static void option_T_list_append(T & option, Option::Priority priority, const std::string & value) {
    if (value.empty()) {
        option.set(priority, value);
        return;
    }
    auto add_priority = priority < option.get_priority() ? option.get_priority() : priority;
    auto val = option.from_string(value);
    bool first = true;
    for (auto & item : val) {
        if (item.empty()) {
            if (first) {
                option.set(priority, item);
            }
        } else {
            auto orig_value = option.get_value();
            orig_value.push_back(item);
            option.set(add_priority, orig_value);
        }
        first = false;
    }
}

}  // namespace libdnf

#endif
