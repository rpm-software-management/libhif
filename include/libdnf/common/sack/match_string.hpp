// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_COMMON_SACK_MATCH_STRING_HPP
#define LIBDNF_COMMON_SACK_MATCH_STRING_HPP


#include "query_cmp.hpp"

#include <string>
#include <vector>


namespace libdnf::sack {


bool match_string(const std::string & value, QueryCmp cmp, const std::string & pattern);
bool match_string(const std::string & value, QueryCmp cmp, const std::vector<std::string> & patterns);
bool match_string(const std::vector<std::string> & values, QueryCmp cmp, const std::string & pattern);
bool match_string(const std::vector<std::string> & values, QueryCmp cmp, const std::vector<std::string> & patterns);


}  // namespace libdnf::sack


#endif
