// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_COMMON_SACK_MATCH_INT64_HPP
#define LIBDNF_COMMON_SACK_MATCH_INT64_HPP


#include "query_cmp.hpp"

#include <cstdint>
#include <vector>


namespace libdnf::sack {


bool match_int64(int64_t value, QueryCmp cmp, int64_t pattern);
bool match_int64(int64_t value, QueryCmp cmp, const std::vector<int64_t> & patterns);
bool match_int64(const std::vector<int64_t> & values, QueryCmp cmp, int64_t pattern);
bool match_int64(const std::vector<int64_t> & values, QueryCmp cmp, const std::vector<int64_t> & patterns);


}  // namespace libdnf::sack

#endif
