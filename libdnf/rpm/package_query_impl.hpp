// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_RPM_PACKAGE_QUERY_IMPL_HPP
#define LIBDNF_RPM_PACKAGE_QUERY_IMPL_HPP

#include "solv/solv_map.hpp"

#include "libdnf/rpm/package_query.hpp"

extern "C" {
#include <solv/solvable.h>
}

namespace libdnf::rpm {


class PackageQuery::Impl {
public:
    static void filter_provides(
        Pool * pool, libdnf::sack::QueryCmp cmp_type, const ReldepList & reldep_list, solv::SolvMap & filter_result);
    static void filter_reldep(
        PackageSet & pkg_set,
        Id libsolv_key,
        libdnf::sack::QueryCmp cmp_type,
        const std::vector<std::string> & patterns);
    static void filter_reldep(
        PackageSet & pkg_set, Id libsolv_key, libdnf::sack::QueryCmp cmp_type, const ReldepList & reldep_list);
    static void filter_reldep(
        PackageSet & pkg_set, Id libsolv_key, libdnf::sack::QueryCmp cmp_type, const PackageSet & package_set);

    /// @param cmp_glob performance optimization - it must be in synchronization with cmp_type
    static void filter_nevra(
        PackageSet & pkg_set,
        const Nevra & pattern,
        bool cmp_glob,
        libdnf::sack::QueryCmp cmp_type,
        solv::SolvMap & filter_result,
        bool with_src);
    static void filter_nevra(
        PackageSet & pkg_set,
        const std::vector<Solvable *> & sorted_solvables,
        const std::string & pattern,
        bool cmp_glob,
        libdnf::sack::QueryCmp cmp_type,
        solv::SolvMap & filter_result);
    /// Provide libdnf::sack::QueryCmp without NOT flag
    static void str2reldep_internal(
        ReldepList & reldep_list, libdnf::sack::QueryCmp cmp_type, bool cmp_glob, const std::string & pattern);
    /// Provide libdnf::sack::QueryCmp without NOT flag
    static void str2reldep_internal(
        ReldepList & reldep_list, libdnf::sack::QueryCmp cmp_type, const std::vector<std::string> & patterns);
};


}  // namespace libdnf::rpm

#endif  // LIBDNF_RPM_PACKAGE_QUERY_IMPL_HPP
