// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_COMPS_GROUP_SACK_IMPL_HPP
#define LIBDNF_COMPS_GROUP_SACK_IMPL_HPP

#include "libdnf/comps/group/sack.hpp"

namespace libdnf::comps {


class GroupSack::Impl {
public:
    explicit Impl();
    ~Impl();

private:
    WeakPtrGuard<GroupSack, false> data_guard;

    friend GroupSack;
};


}  // namespace libdnf::comps


#endif  // LIBDNF_COMPS_GROUP_SACK_IMPL_HPP
