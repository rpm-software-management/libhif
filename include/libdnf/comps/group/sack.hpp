// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_COMPS_GROUP_SACK_HPP
#define LIBDNF_COMPS_GROUP_SACK_HPP

#include "libdnf/common/sack/sack.hpp"
#include "libdnf/common/weak_ptr.hpp"

#include <memory>


namespace libdnf::comps {


class Comps;

class Group;

class GroupQuery;

class GroupSack;
using GroupSackWeakPtr = WeakPtr<GroupSack, false>;


class GroupSack : public libdnf::sack::Sack<Group, GroupQuery> {
public:
    ~GroupSack();

    // Create new query with newly composed groups (using only solvables from currently enabled repositories)
    GroupQuery new_query();

    /// Create WeakPtr to GroupSack
    GroupSackWeakPtr get_weak_ptr();

protected:
    explicit GroupSack(Comps & comps);

private:
    Comps & comps;

    class Impl;
    std::unique_ptr<Impl> p_impl;

    friend Comps;
    friend GroupQuery;
    friend Group;
};


}  // namespace libdnf::comps


#endif  // LIBDNF_COMPS_GROUP_SACK_HPP
