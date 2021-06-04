// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_ADVISORY_ADVISORY_SACK_HPP
#define LIBDNF_ADVISORY_ADVISORY_SACK_HPP

#include "advisory_query.hpp"

#include "libdnf/common/weak_ptr.hpp"
#include "libdnf/rpm/solv/solv_map.hpp"

namespace libdnf {
class Base;
}

namespace libdnf::advisory {

//class AsdvisorySack;

using AdvisorySackWeakPtr = WeakPtr<AdvisorySack, false>;

class AdvisorySack {
public:
    explicit AdvisorySack(libdnf::Base & base);
    ~AdvisorySack();

    /// Create new AdvisoryQuery on advisories loaded in this AdvisorySack
    ///
    /// @return new AdvisoryQuery.
    AdvisoryQuery new_query();

    AdvisorySackWeakPtr get_weak_ptr();

private:
    friend AdvisoryQuery;

    WeakPtrGuard<AdvisorySack, false> data_guard;

    /// Load all advisories present in PackageSack from base. This method is
    /// called automatically when creating a new query and the cached number
    /// of solvables doesn't match the current number in solv sacks pool.
    void load_advisories_from_package_sack();

    libdnf::rpm::solv::SolvMap data_map;

    int cached_solvables_size{0};

    libdnf::Base * base;
};

inline AdvisorySackWeakPtr AdvisorySack::get_weak_ptr() { return AdvisorySackWeakPtr(this, &data_guard); }

}  // namespace libdnf::advisory

#endif
