// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "libdnf/conf/option_string_list.hpp"

#include <regex>

namespace libdnf {

OptionStringList::OptionStringList(const ValueType & default_value)
    : Option(Priority::DEFAULT)
    , icase(false)
    , default_value(default_value)
    , value(default_value) {}

OptionStringList::OptionStringList(const ValueType & default_value, std::string regex, bool icase)
    : Option(Priority::DEFAULT)
    , regex(std::move(regex))
    , icase(icase)
    , default_value(default_value)
    , value(default_value) {
    test(default_value);
}

OptionStringList::OptionStringList(const std::string & default_value) : Option(Priority::DEFAULT), icase(false) {
    this->value = this->default_value = from_string(default_value);
}

OptionStringList::OptionStringList(const std::string & default_value, std::string regex, bool icase)
    : Option(Priority::DEFAULT)
    , regex(std::move(regex))
    , icase(icase) {
    this->default_value = from_string(default_value);
    test(this->default_value);
    value = this->default_value;
}

void OptionStringList::test(const std::vector<std::string> & value) const {
    if (regex.empty()) {
        return;
    }
    std::regex re(
        regex,
        std::regex::nosubs | std::regex::extended | (icase ? std::regex::icase : std::regex_constants::ECMAScript));
    for (const auto & val : value) {
        if (!std::regex_match(val, re)) {
            throw NotAllowedValue(val);
        }
    }
}

OptionStringList::ValueType OptionStringList::from_string(const std::string & value) const {
    std::vector<std::string> tmp;
    auto start = value.find_first_not_of(' ');
    while (start != std::string::npos && start < value.length()) {
        auto end = value.find_first_of(" ,\n", start);
        if (end == std::string::npos) {
            tmp.push_back(value.substr(start));
            break;
        }
        tmp.push_back(value.substr(start, end - start));
        start = value.find_first_not_of(' ', end + 1);
        if (start != std::string::npos && value[start] == ',' && value[end] == ' ') {
            end = start;
            start = value.find_first_not_of(' ', start + 1);
        }
        if (start != std::string::npos && value[start] == '\n' && (value[end] == ' ' || value[end] == ',')) {
            start = value.find_first_not_of(' ', start + 1);
        }
    }
    return tmp;
}

void OptionStringList::set(Priority priority, const ValueType & value) {
    if (is_locked()) {
        throw WriteLocked(get_lock_comment());
    }
    if (priority >= get_priority()) {
        test(value);
        this->value = value;
        set_priority(priority);
    }
}

void OptionStringList::set(Priority priority, const std::string & value) {
    set(priority, from_string(value));
}

std::string OptionStringList::to_string(const ValueType & value) const {
    std::ostringstream oss;
    bool next{false};
    for (auto & val : value) {
        if (next) {
            oss << ", ";
        } else {
            next = true;
        }
        oss << val;
    }
    return oss.str();
}

}  // namespace libdnf
