// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "libdnf/advisory/advisory_sack.hpp"

#include "libdnf/rpm/package_sack_impl.hpp"

#include <solv/dataiterator.h>

namespace libdnf::advisory {

AdvisorySack::AdvisorySack(libdnf::Base & base) : data_map(0), base(&base) {}

AdvisorySack::~AdvisorySack() = default;

AdvisoryQuery AdvisorySack::new_query() {
    auto package_sack = base->get_rpm_package_sack();

    if (cached_solvables_size != package_sack->p_impl->get_nsolvables()) {
        load_advisories_from_package_sack();
    }

    return AdvisoryQuery(this->get_weak_ptr());
};

void AdvisorySack::load_advisories_from_package_sack() {
    auto package_sack = base->get_rpm_package_sack();
    Pool * pool = package_sack->p_impl->get_pool();

    data_map = libdnf::rpm::solv::SolvMap(pool->nsolvables);

    Dataiterator di;

    dataiterator_init(&di, pool, 0, 0, 0, 0, 0);

    // - We want only advisories that have at least one package in them.
    // - Advisories have their own Ids but advisory packages don't.
    // - Keyname UPDATE_COLLECTION refers to packages (name, evr, arch) in advisories,
    //   it is misnamed and should be rather called UPDATE_PACKAGE.
    //
    // This loop finds the first package in an advisory, adds the advisory
    // Id to data_map and then skips it. Without the skip we would iterate
    // over all packages in the advisory and added the same adivisory Id
    // multiple times.
    dataiterator_prepend_keyname(&di, UPDATE_COLLECTION);
    while (dataiterator_step(&di)) {
        data_map.add(di.solvid);
        dataiterator_skip_solvable(&di);
    }
    dataiterator_free(&di);

    cached_solvables_size = pool->nsolvables;
}

}  // namespace libdnf::advisory
