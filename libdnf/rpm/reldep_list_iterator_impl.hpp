// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_RPM_RELDEP_LIST_ITERATOR_IMPL_HPP
#define LIBDNF_RPM_RELDEP_LIST_ITERATOR_IMPL_HPP


#include "libdnf/rpm/reldep_list_impl.hpp"
#include "libdnf/rpm/reldep_list_iterator.hpp"


namespace libdnf::rpm {


class ReldepListIterator::Impl : public solv::IdQueue::iterator {
public:
    Impl(const ReldepList & reldep_list);
    Impl(const ReldepListIterator::Impl & reldep_list_iterator_impl) = default;

    ReldepListIterator::Impl & operator++();

private:
    friend ReldepListIterator;
    const ReldepList & reldep_list;
};


inline ReldepListIterator::Impl::Impl(const ReldepList & reldep_list)
    : solv::IdQueue::iterator(&(reldep_list.p_impl->get_idqueue().get_queue()))
    , reldep_list{reldep_list} {}

inline ReldepListIterator::Impl & ReldepListIterator::Impl::operator++() {
    solv::IdQueue::iterator::operator++();
    return *this;
}


}  // namespace libdnf::rpm


#endif  // LIBDNF_RPM_RELDEP_LIST_ITERATOR_IMPL_HPP
