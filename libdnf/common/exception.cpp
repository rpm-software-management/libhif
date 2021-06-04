// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "libdnf/common/exception.hpp"

#include <algorithm>
#include <charconv>
#include <system_error>


namespace libdnf {


const char * Exception::what() const noexcept {
    try {
        str_what = std::string(get_domain_name()) + "::" + get_name() + ": " + get_description();

        // related data, eg file name
        const char * what = runtime_error::what();
        if (what && what[0] != '\0') {
            str_what += ": ";
            str_what += what;
        }

        return str_what.c_str();
    } catch (...) {
        return runtime_error::what();
    }
}

SystemError::SystemError(int error_code, const std::string & what) : RuntimeError(what), error_code{error_code} {
    char * it = std::copy_n(NAME_PREFIX, NAME_PREFIX_LEN, name);
    auto res = std::to_chars(it, name + sizeof(name) - 1, get_error_code());
    *res.ptr = '\0';
}

SystemError::SystemError(int error_code, const char * what) : RuntimeError(what), error_code{error_code} {
    char * it = std::copy_n(NAME_PREFIX, NAME_PREFIX_LEN, name);
    auto res = std::to_chars(it, name + sizeof(name) - 1, get_error_code());
    *res.ptr = '\0';
}

const char * SystemError::get_description() const noexcept {
    try {
        description = std::system_category().default_error_condition(error_code).message();
    } catch (...) {
        return "Unknown error";
    }
    return description.c_str();
}

std::string format(const std::exception & e, std::size_t level) {
    std::string ret(std::string(level, ' ') + e.what() + '\n');
    try {
        std::rethrow_if_nested(e);
    } catch (const std::exception & e) {
        ret += format(e, level + 1);
    } catch (...) {
    }

    return ret;
}

}  // namespace libdnf
