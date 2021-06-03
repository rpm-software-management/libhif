// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_RPM_PACKAGE_SET_IMPL_HPP
#define LIBDNF_RPM_PACKAGE_SET_IMPL_HPP


#include "libdnf/rpm/package_set.hpp"
#include "libdnf/rpm/solv/solv_map.hpp"

extern "C" {
#include <solv/pool.h>
}

#include "package_sack_impl.hpp"


namespace libdnf::rpm {


class PackageSet::Impl : public solv::SolvMap {
public:
    /// Initialize with an empty map
    //explicit Impl(PackageSack * sack);

    /// Initialize with an empty map
    explicit Impl(const PackageSackWeakPtr & sack);

    /// Clone from an existing map
    explicit Impl(const PackageSackWeakPtr & sack, solv::SolvMap & solv_map);

    /// Copy constructor: clone from an existing PackageSet::Impl
    Impl(const Impl & other);

    /// Move constructor: clone from an existing PackageSet::Impl
    Impl(Impl && other);

    Impl & operator=(const Impl & other);
    Impl & operator=(Impl && other);
    Impl & operator=(const solv::SolvMap & map);
    Impl & operator=(solv::SolvMap && map);

    PackageSackWeakPtr get_sack() const { return sack; }

private:
    friend PackageSet;
    friend PackageQuery;
    PackageSackWeakPtr sack;
};


inline PackageSet::Impl::Impl(const PackageSackWeakPtr & sack)
    : solv::SolvMap::SolvMap(sack->p_impl->pool->nsolvables)
    , sack(sack) {}

inline PackageSet::Impl::Impl(const PackageSackWeakPtr & sack, solv::SolvMap & solv_map)
    : solv::SolvMap::SolvMap(solv_map)
    , sack(sack) {}

inline PackageSet::Impl::Impl(const Impl & other)
    : solv::SolvMap::SolvMap(other.get_map())
    , sack(other.sack) {}

inline PackageSet::Impl::Impl(Impl && other)
    : solv::SolvMap::SolvMap(std::move(other.get_map()))
    , sack(std::move(other.sack)) {}

inline PackageSet::Impl & PackageSet::Impl::operator=(const Impl & other) {
    solv::SolvMap::operator=(other);
    sack = other.sack;
    return *this;
}

inline PackageSet::Impl & PackageSet::Impl::operator=(Impl && other) {
    solv::SolvMap::operator=(std::move(other));
    sack = std::move(other.sack);
    return *this;
}

inline PackageSet::Impl & PackageSet::Impl::operator=(const solv::SolvMap & map) {
    solv::SolvMap::operator=(map);
    return *this;
}

inline PackageSet::Impl & PackageSet::Impl::operator=(solv::SolvMap && map) {
    solv::SolvMap::operator=(std::move(map));
    return *this;
}

}  // namespace libdnf::rpm


#endif  // LIBDNF_RPM_PACKAGE_SET_IMPL_HPP
