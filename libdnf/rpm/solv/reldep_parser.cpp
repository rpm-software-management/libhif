// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "reldep_parser.hpp"

#include <regex>
#include <string>

namespace libdnf::rpm::solv {

static const std::regex RELDEP_REGEX("^(\\S*)\\s*(\\S*)?\\s*(\\S*)$");

static bool set_cmp_type(Reldep::CmpType * cmp_type, std::string cmp_type_string, long int length) {
    if (length == 2) {
        // The second character must be '='
        if (cmp_type_string[1] != '=') {
            return false;
        }
        if (cmp_type_string[0] == '<') {
            *cmp_type = Reldep::CmpType::LTE;
            return true;
        } else if (cmp_type_string[0] == '>') {
            *cmp_type = Reldep::CmpType::GTE;
            return true;
        } else {
            return false;
        }
    } else if (length == 1) {
        if (cmp_type_string[0] == '>') {
            *cmp_type = Reldep::CmpType::GT;
            return true;
        } else if (cmp_type_string[0] == '<') {
            *cmp_type = Reldep::CmpType::LT;
            return true;
        } else if (cmp_type_string[0] == '=') {
            *cmp_type = Reldep::CmpType::EQ;
            return true;
        } else {
            return false;
        }
    }
    return false;
}

bool ReldepParser::parse(const std::string & reldep_str) {
    enum { NAME = 1, CMP_TYPE = 2, EVR = 3 };
    std::smatch match;
    if (!std::regex_match(reldep_str, match, RELDEP_REGEX)) {
        return false;
    } else {
        std::ssub_match cmp_type_sub_match = match[CMP_TYPE];
        std::ssub_match evr_sub_match = match[EVR];
        auto cmp_type_length = cmp_type_sub_match.length();
        auto evr_length = evr_sub_match.length();
        // Only evr and cmp_type together or only name is a valid reldep
        if ((cmp_type_length && evr_length) || (!cmp_type_length && !evr_length)) {
            name = match[NAME].str();
            if (cmp_type_length) {
                evr = evr_sub_match.str();
                auto cmp_type_string = cmp_type_sub_match.str();
                return set_cmp_type(&cmp_type, cmp_type_string, cmp_type_length);
            } else {
                cmp_type = libdnf::rpm::Reldep::CmpType::NONE;
                evr.clear();
                return true;
            }
        } else {
            return false;
        }
    }
}

}  // namespace libdnf::rpm::solv
