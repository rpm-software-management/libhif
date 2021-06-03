// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "libdnf/rpm/package_set.hpp"

#include "package_set_impl.hpp"

#include "libdnf/rpm/package_sack.hpp"
#include "libdnf/rpm/package_set_iterator.hpp"
#include "libdnf/rpm/solv/solv_map.hpp"


namespace libdnf::rpm {


PackageSet::PackageSet(const PackageSackWeakPtr & sack) : p_impl(new Impl(sack)) {}

PackageSet::PackageSet(const PackageSet & other) : p_impl(new Impl(*other.p_impl)) {}


PackageSet::PackageSet(PackageSet && other) noexcept : p_impl(new Impl(std::move(*other.p_impl))) {}


PackageSet::PackageSet(const PackageSackWeakPtr & sack, libdnf::rpm::solv::SolvMap & solv_map) : p_impl(new Impl(sack, solv_map)) {}


PackageSet::~PackageSet() = default;

PackageSet & PackageSet::operator=(const PackageSet & other) {
    *p_impl = *other.p_impl;
    return *this;
}

PackageSet & PackageSet::operator=(PackageSet && other) {
    *p_impl = std::move(*other.p_impl);
    return *this;
}

PackageSet::iterator PackageSet::begin() const {
    PackageSet::iterator it(*this);
    it.begin();
    return it;
}


PackageSet::iterator PackageSet::end() const {
    PackageSet::iterator it(*this);
    it.end();
    return it;
}


PackageSet & PackageSet::operator|=(const PackageSet & other) {
    if (p_impl->sack != other.p_impl->sack) {
        throw UsedDifferentSack(
            "Cannot perform the action with PackageSet instances initialized with different PackageSacks");
    }
    *p_impl |= *other.p_impl;
    return *this;
}


PackageSet & PackageSet::operator-=(const PackageSet & other) {
    if (p_impl->sack != other.p_impl->sack) {
        throw UsedDifferentSack(
            "Cannot perform the action with PackageSet instances initialized with different PackageSacks");
    }
    *p_impl -= *other.p_impl;
    return *this;
}


PackageSet & PackageSet::operator&=(const PackageSet & other) {
    if (p_impl->sack != other.p_impl->sack) {
        throw UsedDifferentSack(
            "Cannot perform the action with PackageSet instances initialized with different PackageSacks");
    }
    *p_impl &= *other.p_impl;
    return *this;
}


void PackageSet::clear() noexcept {
    p_impl->clear();
}


bool PackageSet::empty() const noexcept {
    return p_impl->empty();
}


std::size_t PackageSet::size() const noexcept {
    return p_impl->size();
}


void PackageSet::swap(PackageSet & other) noexcept {
    p_impl.swap(other.p_impl);
}


void PackageSet::add(const Package & pkg) {
    p_impl->add(pkg.get_id().id);
}


bool PackageSet::contains(const Package & pkg) const noexcept {
    return p_impl->contains(pkg.get_id().id);
}


void PackageSet::remove(const Package & pkg) {
    p_impl->remove(pkg.get_id().id);
}


PackageSackWeakPtr PackageSet::get_sack() const {
    return p_impl->get_sack();
}


}  // namespace libdnf::rpm
