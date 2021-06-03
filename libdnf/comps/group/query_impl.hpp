// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_COMPS_GROUP_QUERY_IMPL_HPP
#define LIBDNF_COMPS_GROUP_QUERY_IMPL_HPP

#include "libdnf/comps/group/query.hpp"


namespace libdnf::comps {


class GroupQuery::Impl {
public:
    ~Impl() = default;

private:
    WeakPtrGuard<GroupQuery, false> data_guard;

    friend GroupQuery;
};


}  // namespace libdnf::comps


#endif  // LIBDNF_COMPS_GROUP_QUERY_HPP
