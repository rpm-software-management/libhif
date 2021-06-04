// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "libdnf/rpm/package_set_iterator_impl.hpp"


namespace libdnf::rpm {


PackageSetIterator::PackageSetIterator(const PackageSet & package_set) : p_impl{new Impl(package_set)} {}

PackageSetIterator::PackageSetIterator(const PackageSetIterator & other) : p_impl{new Impl(*other.p_impl)} {}

PackageSetIterator::~PackageSetIterator() {}

void PackageSetIterator::begin() {
    p_impl->begin();
}


void PackageSetIterator::end() {
    p_impl->end();
}


Package PackageSetIterator::operator*() {
    return {p_impl->package_set.get_sack(), libdnf::rpm::PackageId(**p_impl)};
}


PackageSetIterator & PackageSetIterator::operator++() {
    ++*p_impl;
    return *this;
}


PackageSetIterator PackageSetIterator::operator++(int) {
    PackageSetIterator it(*this);
    ++*p_impl;
    return it;
}


bool PackageSetIterator::operator==(const PackageSetIterator & other) const {
    return *p_impl == *other.p_impl;
}


bool PackageSetIterator::operator!=(const PackageSetIterator & other) const {
    return *p_impl != *other.p_impl;
}


}  // namespace libdnf::rpm
