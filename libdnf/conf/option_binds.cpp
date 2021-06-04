// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "libdnf/conf/option_binds.hpp"

#include <utility>

namespace libdnf {

// ========== OptionBinds::Item class ===============

OptionBinds::Item::Item(
    Option & option, NewStringFunc new_string_func, GetValueStringFunc get_value_string_func, bool add_value)
    : option(&option)
    , new_str_func(std::move(new_string_func))
    , get_value_str_func(std::move(get_value_string_func))
    , is_append_option(add_value) {}

OptionBinds::Item::Item(Option & option) : option(&option) {}

Option::Priority OptionBinds::Item::get_priority() const {
    return option->get_priority();
}

void OptionBinds::Item::new_string(Option::Priority priority, const std::string & value) {
    if (new_str_func) {
        new_str_func(priority, value);
    } else {
        option->set(priority, value);
    }
}

std::string OptionBinds::Item::get_value_string() const {
    if (get_value_str_func) {
        return get_value_str_func();
    }
    return option->get_value_string();
}

bool OptionBinds::Item::get_is_append_option() const {
    return is_append_option;
}


// =========== OptionBinds class ===============

OptionBinds::Item & OptionBinds::at(const std::string & id) {
    auto item = items.find(id);
    if (item == items.end()) {
        throw OptionNotFound(id);
    }
    return item->second;
}

const OptionBinds::Item & OptionBinds::at(const std::string & id) const {
    auto item = items.find(id);
    if (item == items.end()) {
        throw OptionNotFound(id);
    }
    return item->second;
}

OptionBinds::Item & OptionBinds::add(
    const std::string & id,
    Option & option,
    Item::NewStringFunc new_string_func,
    Item::GetValueStringFunc get_value_string_func,
    bool add_value) {
    auto item = items.find(id);
    if (item != items.end()) {
        throw OptionAlreadyExists(id);
    }
    auto res = items.emplace(id, Item(option, std::move(new_string_func), std::move(get_value_string_func), add_value));
    return res.first->second;
}

OptionBinds::Item & OptionBinds::add(const std::string & id, Option & option) {
    auto item = items.find(id);
    if (item != items.end()) {
        throw OptionAlreadyExists(id);
    }
    auto res = items.emplace(id, Item(option));
    return res.first->second;
}

}  // namespace libdnf
