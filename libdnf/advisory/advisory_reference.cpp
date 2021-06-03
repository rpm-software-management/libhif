// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "libdnf/advisory/advisory_reference.hpp"

#include "libdnf/logger/logger.hpp"
#include "libdnf/rpm/package_sack_impl.hpp"

#include <solv/chksum.h>
#include <solv/repo.h>
#include <solv/util.h>

namespace libdnf::advisory {

inline const char * get_str_from_pool(Id keyname, Id advisory, Pool * pool, int index) {
    Dataiterator di;
    const char * str = NULL;
    int count = 0;

    dataiterator_init(&di, pool, 0, advisory, UPDATE_REFERENCE, 0, 0);
    while (dataiterator_step(&di)) {
        dataiterator_setpos(&di);
        if (count++ == index) {
            str = pool_lookup_str(pool, SOLVID_POS, keyname);
            break;
        }
    }
    dataiterator_free(&di);

    return str;
}

AdvisoryReference::AdvisoryReference(const libdnf::rpm::PackageSackWeakPtr & sack, AdvisoryId advisory, int index)
    : sack(sack)
    , advisory(advisory)
    , index(index) {}

std::string AdvisoryReference::get_id() const {
    Pool * pool = sack->p_impl->get_pool();
    return std::string(get_str_from_pool(UPDATE_REFERENCE_ID, advisory.id, pool, index));
}
AdvisoryReference::Type AdvisoryReference::get_type() const {
    Pool * pool = sack->p_impl->get_pool();
    const char * type = get_str_from_pool(UPDATE_REFERENCE_TYPE, advisory.id, pool, index);

    if (type == NULL)
        return Type::UNKNOWN;
    if (!g_strcmp0(type, "bugzilla"))
        return Type::BUGZILLA;
    if (!g_strcmp0(type, "cve"))
        return Type::CVE;
    if (!g_strcmp0(type, "vendor"))
        return Type::VENDOR;
    return Type::UNKNOWN;
}
std::string AdvisoryReference::get_title() const {
    Pool * pool = sack->p_impl->get_pool();
    return std::string(get_str_from_pool(UPDATE_REFERENCE_TITLE, advisory.id, pool, index));
}
std::string AdvisoryReference::get_url() const {
    Pool * pool = sack->p_impl->get_pool();
    return std::string(get_str_from_pool(UPDATE_REFERENCE_HREF, advisory.id, pool, index));
}

}  // namespace libdnf::advisory
