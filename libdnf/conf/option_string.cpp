// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "libdnf/conf/option_string.hpp"

#include <regex>

namespace libdnf {

OptionString::OptionString(const std::string & default_value)
    : Option(Priority::DEFAULT)
    , icase(false)
    , default_value(default_value)
    , value(default_value) {}

OptionString::OptionString(const char * default_value) : icase(false) {
    if (default_value) {
        this->value = this->default_value = default_value;
        set_priority(Priority::DEFAULT);
    }
}

OptionString::OptionString(const std::string & default_value, std::string regex, bool icase)
    : Option(Priority::DEFAULT)
    , regex(std::move(regex))
    , icase(icase)
    , default_value(default_value)
    , value(default_value) {
    test(default_value);
}

OptionString::OptionString(const char * default_value, std::string regex, bool icase)
    : regex(std::move(regex))
    , icase(icase) {
    if (default_value) {
        this->default_value = default_value;
        test(this->default_value);
        this->value = this->default_value;
        set_priority(Priority::DEFAULT);
    }
}

void OptionString::test(const std::string & value) const {
    if (regex.empty()) {
        return;
    }
    std::regex re(
        regex,
        std::regex::nosubs | std::regex::extended | (icase ? std::regex::icase : std::regex_constants::ECMAScript));
    if (!std::regex_match(value, re)) {
        throw NotAllowedValue(value);
    }
}

void OptionString::set(Priority priority, const std::string & value) {
    if (is_locked()) {
        throw WriteLocked(get_lock_comment());
    }
    if (priority >= get_priority()) {
        test(value);
        this->value = value;
        set_priority(priority);
    }
}

const std::string & OptionString::get_value() const {
    if (get_priority() == Priority::EMPTY) {
        throw ValueNotSet();
    }
    return value;
}

}  // namespace libdnf
