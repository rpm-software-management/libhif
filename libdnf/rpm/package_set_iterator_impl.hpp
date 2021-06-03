// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_RPM_PACKAGE_SET_ITERATOR_IMPL_HPP
#define LIBDNF_RPM_PACKAGE_SET_ITERATOR_IMPL_HPP


#include "libdnf/rpm/package_set_impl.hpp"
#include "libdnf/rpm/package_set_iterator.hpp"


namespace libdnf::rpm {


class PackageSetIterator::Impl : public solv::SolvMap::iterator {
public:
    Impl(const PackageSet & package_set);
    Impl(const PackageSetIterator::Impl & package_set_iterator_impl) = default;

    PackageSetIterator::Impl & operator++();

private:
    friend PackageSetIterator;
    const PackageSet & package_set;
};


inline PackageSetIterator::Impl::Impl(const PackageSet & package_set)
    : solv::SolvMap::iterator(package_set.p_impl->get_map())
    , package_set{package_set} {}

inline PackageSetIterator::Impl & PackageSetIterator::Impl::operator++() {
    solv::SolvMap::iterator::operator++();
    return *this;
}


}  // namespace libdnf::rpm


#endif  // LIBDNF_RPM_PACKAGE_SET_ITERATOR_IMPL_HPP
