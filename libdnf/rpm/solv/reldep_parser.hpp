// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_RPM_SOLV_RELDEP_PARSER_HPP
#define LIBDNF_RPM_SOLV_RELDEP_PARSER_HPP

#include "libdnf/rpm/reldep.hpp"

#include <string>


namespace libdnf::rpm::solv {

struct ReldepParser {
public:
    /// @brief Parse realdep std::string into thee elements (name, evr, and comparison type), and transforms into libdnf::rpm::Reldep::ComparisonType.
    /// If parsing is not succesfull, the object contains a garbage.
    ///
    /// @param reldepStr p_reldepStr: std::string & that represent reldep
    /// @return bool - true if parsing was succesful
    bool parse(const std::string & reldep_str);
    const std::string & get_name() const noexcept;
    const char * get_name_cstr() const noexcept;
    const std::string & get_evr() const noexcept;
    const char * get_evr_cstr() const noexcept;
    libdnf::rpm::Reldep::CmpType get_cmp_type() const noexcept;

private:
    std::string name;
    std::string evr;
    libdnf::rpm::Reldep::CmpType cmp_type{libdnf::rpm::Reldep::CmpType::NONE};
};


inline const std::string & ReldepParser::get_name() const noexcept {
    return name;
}

inline const std::string & ReldepParser::get_evr() const noexcept {
    return evr;
}

inline libdnf::rpm::Reldep::CmpType ReldepParser::get_cmp_type() const noexcept {
    return cmp_type;
}

inline const char * ReldepParser::get_name_cstr() const noexcept {
    return name.empty() ? nullptr : name.c_str();
}

inline const char * ReldepParser::get_evr_cstr() const noexcept {
    return evr.empty() ? nullptr : evr.c_str();
}

}  // namespace libdnf::rpm::solv

#endif  // LIBDNF_RPM_SOLV_RELDEP_PARSER_HPP
