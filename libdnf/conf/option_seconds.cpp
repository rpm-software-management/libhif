// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "libdnf/conf/option_seconds.hpp"

namespace libdnf {

OptionSeconds::OptionSeconds(ValueType default_value, ValueType min, ValueType max)
    : OptionNumber(default_value, min, max) {}

OptionSeconds::OptionSeconds(ValueType default_value, ValueType min) : OptionNumber(default_value, min) {}

OptionSeconds::OptionSeconds(ValueType default_value) : OptionNumber(default_value, -1) {}

OptionSeconds::ValueType OptionSeconds::from_string(const std::string & value) const {
    constexpr int seconds_in_minute = 60;
    constexpr int minutes_in_hour = 60;
    constexpr int hours_in_day = 24;
    if (value.empty()) {
        throw InvalidValue(value);
    }

    if (value == "-1" || value == "never") {  // Special cache timeout, meaning never
        return -1;
    }

    std::size_t idx;
    auto res = std::stod(value, &idx);
    if (res < 0) {
        throw NegativeValue(value);
    }

    if (idx < value.length()) {
        if (idx < value.length() - 1) {
            throw InvalidValue(value);
        }
        switch (value.back()) {
            case 's':
            case 'S':
                break;
            case 'm':
            case 'M':
                res *= seconds_in_minute;
                break;
            case 'h':
            case 'H':
                res *= seconds_in_minute * minutes_in_hour;
                break;
            case 'd':
            case 'D':
                res *= seconds_in_minute * minutes_in_hour * hours_in_day;
                break;
            default:
                throw UnknownUnit(std::string(&value.back(), 1));
        }
    }

    return static_cast<ValueType>(res);
}

void OptionSeconds::set(Priority priority, const std::string & value) {
    set(priority, from_string(value));
}

}  // namespace libdnf
