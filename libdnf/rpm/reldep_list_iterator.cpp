// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "libdnf/rpm/reldep_list_iterator_impl.hpp"


namespace libdnf::rpm {


ReldepListIterator::ReldepListIterator(const ReldepList & reldep_list) : p_impl(new Impl(reldep_list)) {}

ReldepListIterator::ReldepListIterator(const ReldepListIterator & other) : p_impl(new Impl(*other.p_impl)) {}

ReldepListIterator::~ReldepListIterator() {}

void ReldepListIterator::begin() {
    p_impl->begin();
}


void ReldepListIterator::end() {
    p_impl->end();
}


Reldep ReldepListIterator::operator*() {
    return {p_impl->reldep_list.get_sack(), ReldepId(**p_impl)};
}


ReldepListIterator & ReldepListIterator::operator++() {
    ++*p_impl;
    return *this;
}


ReldepListIterator ReldepListIterator::operator++(int) {
    ReldepListIterator it(*this);
    ++*p_impl;
    return it;
}


bool ReldepListIterator::operator==(const ReldepListIterator & other) const {
    return *p_impl == *other.p_impl;
}


bool ReldepListIterator::operator!=(const ReldepListIterator & other) const {
    return *p_impl != *other.p_impl;
}


}  // namespace libdnf::rpm
