// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_UTILS_UTILS_INTERNAL_HPP
#define LIBDNF_UTILS_UTILS_INTERNAL_HPP


extern "C" {
#include <solv/pool.h>
}

#include <ctype.h>

#include <cstring>
#include <string>


namespace libdnf::utils {

constexpr const char * SOLVABLE_NAME_ADVISORY_PREFIX = "patch:";
constexpr size_t SOLVABLE_NAME_ADVISORY_PREFIX_LENGTH = std::char_traits<char>::length(SOLVABLE_NAME_ADVISORY_PREFIX);

inline bool is_glob_pattern(const char * pattern) {
    return strpbrk(pattern, "*[?") != nullptr;
}

inline bool is_package(const Pool * pool, Id solvable_id) {
    Solvable * solvable = pool_id2solvable(pool, solvable_id);
    const char * solvable_name = pool_id2str(pool, solvable->name);
    if (!solvable_name) {
        return true;
    }
    return strncmp(solvable_name, SOLVABLE_NAME_ADVISORY_PREFIX, SOLVABLE_NAME_ADVISORY_PREFIX_LENGTH) != 0;
}

/// @brief Test if pattern is file path
/// Return true if pattern start with "/" or pattern[0] == '*' && pattern[1] == '/'
static inline bool is_file_pattern(const std::string & pattern) {
    return pattern[0] == '/' || (pattern[0] == '*' && pattern[1] == '/');
}

inline std::string to_lowercase(const std::string & source) {
    auto length = source.size();
    std::string result;
    result.reserve(length);
    for (unsigned index = 0; index < length; ++index) {
        result += static_cast<char>(tolower(source[index]));
    }
    return result;
}

inline Id id_to_lowercase_id(Pool * pool, const char * name_cstring, int create) {
    int name_length = static_cast<int>(strlen(name_cstring));
    auto tmp_name_cstring = pool_alloctmpspace(pool, name_length);
    for (int index = 0; index < name_length; ++index) {
        tmp_name_cstring[index] = static_cast<char>(tolower(name_cstring[index]));
    }
    return pool_strn2id(pool, tmp_name_cstring, static_cast<unsigned int>(name_length), create);
}

inline Id id_to_lowercase_id(Pool * pool, Id id_input, int create) {
    auto name_cstring = pool_id2str(pool, id_input);
    return id_to_lowercase_id(pool, name_cstring, create);
}


}  // namespace libdnf::utils


#endif  // LIBDNF_UTILS_UTILS_INTERNAL_HPP
