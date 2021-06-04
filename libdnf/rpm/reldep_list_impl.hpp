// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_RPM_RELDEP_LIST_IMPL_HPP
#define LIBDNF_RPM_RELDEP_LIST_IMPL_HPP

#include "solv/id_queue.hpp"

#include "libdnf/rpm/reldep_list.hpp"

namespace libdnf::rpm {

class ReldepList::Impl {
public:
    Impl(const ReldepList::Impl & src) = default;
    Impl(const PackageSackWeakPtr & sack) : sack(sack) {}
    Impl(const PackageSackWeakPtr & sack, libdnf::rpm::solv::IdQueue queue_src) : sack(sack), queue(queue_src) {}
    ~Impl() = default;

    PackageSack * get_sack() const { return sack.get(); }
    libdnf::rpm::solv::IdQueue & get_idqueue() { return queue; }

private:
    friend class ReldepList;
    friend Package;
    PackageSackWeakPtr sack;
    libdnf::rpm::solv::IdQueue queue;
};

}  // namespace libdnf::rpm

#endif  // LIBDNF_RPM_RELDEP_LIST_IMPL_HPP
