// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_COMMON_SACK_QUERY_CMP_HPP
#define LIBDNF_COMMON_SACK_QUERY_CMP_HPP


#include <cstdint>
#include <type_traits>


namespace libdnf::sack {


// https://docs.djangoproject.com/en/3.0/ref/models/querysets/#field-lookups
enum class QueryCmp : uint32_t {
    // MODIFIERS
    NOT = (1 << 0),    // negation
    ICASE = (1 << 1),  // case-insensitive match

    // NULL
    ISNULL = (1 << 7),  // value is NULL/None

    // NUMBERS
    EQ = (1 << 8),     // equal; identical meaning to EXACT
    NEQ = (NOT | EQ),  // not equal
    GT = (1 << 9),     // greater than
    GTE = (GT | EQ),   // greater than or equal to
    LT = (1 << 10),    // less than
    LTE = (LT | EQ),   // less than or equal to

    // STRINGS
    EXACT = (EQ),                        // case-sensitive exact match
    IEXACT = (ICASE | EXACT),            // case-insensitive exact match
    NOT_IEXACT = (NOT | IEXACT),         // not case-insensitive exact match
    CONTAINS = (1 << 16),                // case-sensitive containment test
    NOT_CONTAINS = (NOT | CONTAINS),     // not case-sensitive containment test
    ICONTAINS = (ICASE | CONTAINS),      // case-insensitive containment test
    NOT_ICONTAINS = (NOT | ICONTAINS),   // not case-insensitive containment test
    STARTSWITH = (1 << 17),              // case-sensitive starts-with
    ISTARTSWITH = (ICASE | STARTSWITH),  // case-insensitive starts-with
    ENDSWITH = (1 << 18),                // case-sensitive ends-with
    IENDSWITH = (ICASE | ENDSWITH),      // case-insensitive ends-with
    REGEX = (1 << 19),                   // case-sensitive regular expression match.
    IREGEX = (ICASE | REGEX),            // case-insensitive regular expression match.
    GLOB = (1 << 20),                    // case-sensitive glob match
    NOT_GLOB = (NOT | GLOB),               // not case-sensitive glob match
    IGLOB = (ICASE | GLOB),              // case-insensitive glob match
    NOT_IGLOB = (NOT | IGLOB),           // not case-insensitive glob match
};


inline QueryCmp operator|(QueryCmp lhs, QueryCmp rhs) {
    return static_cast<QueryCmp>(
        static_cast<std::underlying_type<QueryCmp>::type>(lhs) |
        static_cast<std::underlying_type<QueryCmp>::type>(rhs));
}


inline QueryCmp operator&(QueryCmp lhs, QueryCmp rhs) {
    return static_cast<QueryCmp>(
        static_cast<std::underlying_type<QueryCmp>::type>(lhs) &
        static_cast<std::underlying_type<QueryCmp>::type>(rhs));
}

// TODO(jmracek) Change the strance operator
inline QueryCmp operator-(QueryCmp lhs, QueryCmp rhs) {
    return static_cast<QueryCmp>(
        static_cast<std::underlying_type<QueryCmp>::type>(lhs) -
        static_cast<std::underlying_type<QueryCmp>::type>(rhs));
}

}  // namespace libdnf::sack


#endif
