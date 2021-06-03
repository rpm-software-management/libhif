// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "libdnf/conf/option_bool.hpp"

#include <sstream>

namespace libdnf {

OptionBool::OptionBool(const OptionBool & src) : Option(src), default_value(src.default_value), value(src.value) {
    if (src.true_values) {
        true_values = std::make_unique<std::vector<std::string>>(*src.true_values);
    }
    if (src.false_values) {
        false_values = std::make_unique<std::vector<std::string>>(*src.false_values);
    }
}

OptionBool & OptionBool::operator=(const OptionBool & src) {
    if (is_locked()) {
        throw WriteLocked(get_lock_comment());
    }
    if (this == &src) {
        return *this;
    }
    if (src.true_values) {
        true_values = std::make_unique<std::vector<std::string>>(*src.true_values);
    }
    if (src.false_values) {
        false_values = std::make_unique<std::vector<std::string>>(*src.false_values);
    }
    default_value = src.default_value;
    value = src.value;
    return *this;
}


OptionBool::OptionBool(
    bool default_value, const std::vector<std::string> & true_vals, const std::vector<std::string> & false_vals)
    : Option(Priority::DEFAULT)
    , true_values(std::make_unique<std::vector<std::string>>(true_vals))
    , false_values(std::make_unique<std::vector<std::string>>(false_vals))
    , default_value(default_value)
    , value(default_value) {}

OptionBool::OptionBool(bool default_value)
    : Option(Priority::DEFAULT)
    , default_value(default_value)
    , value(default_value) {}

bool OptionBool::from_string(const std::string & value) const {
    auto tmp_value = value;
    // Case insensitive conversion. Convert input value to lower case first.
    for (auto & ch : tmp_value) {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
    for (auto & false_value : get_false_values()) {
        if (tmp_value == false_value) {
            return false;
        }
    }
    for (auto & true_value : get_true_values()) {
        if (tmp_value == true_value) {
            return true;
        }
    }
    throw InvalidValue(value);
}

void OptionBool::set(Priority priority, bool value) {
    if (is_locked()) {
        throw WriteLocked(get_lock_comment());
    }
    if (priority >= get_priority()) {
        this->value = value;
        set_priority(priority);
    }
}

void OptionBool::set(Priority priority, const std::string & value) {
    set(priority, from_string(value));
}

std::string OptionBool::to_string(bool value) const {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

}  // namespace libdnf
