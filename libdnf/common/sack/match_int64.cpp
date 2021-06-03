// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "libdnf/common/sack/match_int64.hpp"

#include "libdnf/common/exception.hpp"

#include <stdexcept>


namespace libdnf::sack {


bool match_int64(int64_t value, QueryCmp cmp, int64_t pattern) {
    bool result = false;
    switch (cmp) {
        case QueryCmp::EQ:
            result = value == pattern;
            break;
        case QueryCmp::NEQ:
            result = value != pattern;
            break;
        case QueryCmp::LT:
            result = value < pattern;
            break;
        case QueryCmp::LTE:
            result = value <= pattern;
            break;
        case QueryCmp::GT:
            result = value > pattern;
            break;
        case QueryCmp::GTE:
            result = value >= pattern;
            break;
        case QueryCmp::IEXACT:
        case QueryCmp::NOT_IEXACT:
        case QueryCmp::GLOB:
        case QueryCmp::NOT_GLOB:
        case QueryCmp::IGLOB:
        case QueryCmp::NOT_IGLOB:
        case QueryCmp::REGEX:
        case QueryCmp::IREGEX:
        case QueryCmp::CONTAINS:
        case QueryCmp::NOT_CONTAINS:
        case QueryCmp::ICONTAINS:
        case QueryCmp::NOT_ICONTAINS:
        case QueryCmp::STARTSWITH:
        case QueryCmp::ISTARTSWITH:
        case QueryCmp::ENDSWITH:
        case QueryCmp::IENDSWITH:
        case QueryCmp::ISNULL:
            throw std::runtime_error("Unsupported operator");
            break;
        case QueryCmp::NOT:
        case QueryCmp::ICASE:
            throw std::runtime_error("Operator flag cannot be used standalone");
            break;
    }
    return result;
}


bool match_int64(int64_t value, QueryCmp cmp, const std::vector<int64_t> & patterns) {
    bool result = false;
    for (auto & pattern : patterns) {
        if (match_int64(value, cmp, pattern)) {
            result = true;
            break;
        }
    }
    return result;
}


bool match_int64(const std::vector<int64_t> & values, QueryCmp cmp, int64_t pattern) {
    bool result = false;
    for (auto & value : values) {
        if (match_int64(value, cmp, pattern)) {
            result = true;
            break;
        }
    }
    return result;
}


bool match_int64(const std::vector<int64_t> & values, QueryCmp cmp, const std::vector<int64_t> & patterns) {
    bool result = false;
    bool found = false;
    for (auto & value : values) {
        for (auto & pattern : patterns) {
            if (match_int64(value, cmp, pattern)) {
                result = true;
                found = true;
                break;
            }
        }
        if (found) {
            break;
        }
    }
    return result;
}


}  // namespace libdnf::sack
