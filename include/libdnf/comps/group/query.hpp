// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_COMPS_GROUP_QUERY_HPP
#define LIBDNF_COMPS_GROUP_QUERY_HPP

#include "libdnf/common/sack/query.hpp"
#include "libdnf/common/weak_ptr.hpp"
#include "libdnf/comps/group/group.hpp"

#include <memory>

namespace libdnf::comps {


class GroupQuery;
using GroupQueryWeakPtr = WeakPtr<GroupQuery, false>;

class GroupSack;
using GroupSackWeakPtr = WeakPtr<GroupSack, false>;


class GroupQuery : public libdnf::sack::Query<Group> {
public:
    explicit GroupQuery(const GroupQuery & query);
    ~GroupQuery();

    GroupQuery & filter_groupid(const std::string & pattern, sack::QueryCmp cmp = libdnf::sack::QueryCmp::EQ);
    GroupQuery & filter_groupid(
        const std::vector<std::string> & patterns, sack::QueryCmp cmp = libdnf::sack::QueryCmp::EQ);
    GroupQuery & filter_name(const std::string & pattern, sack::QueryCmp cmp = libdnf::sack::QueryCmp::EQ);
    GroupQuery & filter_name(
        const std::vector<std::string> & patterns, sack::QueryCmp cmp = libdnf::sack::QueryCmp::EQ);
    GroupQuery & filter_uservisible(bool value);
    GroupQuery & filter_default(bool value);
    GroupQuery & filter_installed(bool value);

    /// Create WeakPtr to GroupQuery
    GroupQueryWeakPtr get_weak_ptr();

protected:
    explicit GroupQuery(GroupSack * sack);
    GroupQuery(GroupQuery && query);

private:
    struct F {
        static std::string groupid(const Group & obj) { return obj.get_groupid(); }
        static std::string name(const Group & obj) { return obj.get_name(); }
        static bool is_uservisible(const Group & obj) { return obj.get_uservisible(); }
        static bool is_default(const Group & obj) { return obj.get_default(); }
        static bool is_installed(const Group & obj) { return obj.get_installed(); }
    };

    GroupSackWeakPtr sack;

    class Impl;
    std::unique_ptr<Impl> p_impl;

    friend Group;
    friend GroupSack;
};


inline GroupQuery & GroupQuery::filter_groupid(const std::string & pattern, sack::QueryCmp cmp) {
    filter(F::groupid, pattern, cmp);
    return *this;
}


inline GroupQuery & GroupQuery::filter_groupid(const std::vector<std::string> & patterns, sack::QueryCmp cmp) {
    filter(F::groupid, patterns, cmp);
    return *this;
}


inline GroupQuery & GroupQuery::filter_name(const std::string & pattern, sack::QueryCmp cmp) {
    filter(F::name, pattern, cmp);
    return *this;
}


inline GroupQuery & GroupQuery::filter_name(const std::vector<std::string> & patterns, sack::QueryCmp cmp) {
    filter(F::name, patterns, cmp);
    return *this;
}


inline GroupQuery & GroupQuery::filter_default(bool value) {
    filter(F::is_default, value, sack::QueryCmp::EQ);
    return *this;
}


inline GroupQuery & GroupQuery::filter_uservisible(bool value) {
    filter(F::is_uservisible, value, sack::QueryCmp::EQ);
    return *this;
}


inline GroupQuery & GroupQuery::filter_installed(bool value) {
    filter(F::is_installed, value, sack::QueryCmp::EQ);
    return *this;
}


}  // namespace libdnf::comps

#endif  // LIBDNF_COMPS_GROUP_QUERY_HPP
