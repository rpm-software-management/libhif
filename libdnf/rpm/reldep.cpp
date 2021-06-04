// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "libdnf/rpm/reldep.hpp"

#include "package_sack_impl.hpp"
#include "solv/reldep_parser.hpp"

// workaround, libsolv lacks 'extern "C"' in its header file
extern "C" {
#include <solv/pool.h>
#include <solv/pool_parserpmrichdep.h>
#include <solv/util.h>
}

#include <stdexcept>

namespace libdnf::rpm {


Reldep::Reldep(PackageSack * sack, ReldepId dependency_id) : sack(sack->get_weak_ptr()), id(dependency_id) {}

Reldep::Reldep(PackageSack * sack, const char * name, const char * version, CmpType cmp_type)
    : sack(sack->get_weak_ptr()) {
    id = get_reldep_id(sack, name, version, cmp_type);
}

Reldep::Reldep(const PackageSackWeakPtr & sack, const std::string & reldep_string) : sack(sack) {
    id = get_reldep_id(sack.get(), reldep_string);
}

Reldep::Reldep(Reldep && reldep) : sack(std::move(reldep.sack)), id(std::move(reldep.id)) {}

const char * Reldep::get_name() const {
    return pool_id2str(sack->p_impl->pool, id.id);
}
const char * Reldep::get_relation() const {
    return pool_id2rel(sack->p_impl->pool, id.id);
}
const char * Reldep::get_version() const {
    return pool_id2evr(sack->p_impl->pool, id.id);
}
std::string Reldep::to_string() {
    auto * cstring = pool_dep2str(sack->p_impl->pool, id.id);
    return cstring ? std::string(cstring) : std::string();
}

ReldepId Reldep::get_reldep_id(PackageSack * sack, const char * name, const char * version, CmpType cmp_type, int create) {
    static_assert(
        static_cast<int>(Reldep::CmpType::EQ) == REL_EQ, "Reldep::ComparisonType::EQ is not identical to solv/REL_EQ");
    static_assert(
        static_cast<int>(Reldep::CmpType::LT) == REL_LT, "Reldep::ComparisonType::LT is not identical to solv/REL_LT");
    static_assert(
        static_cast<int>(Reldep::CmpType::GT) == REL_GT, "Reldep::ComparisonType::GT is not identical to solv/REL_GT");
    Pool * pool = sack->p_impl->pool;
    Id id = pool_str2id(pool, name, create);
    if (id == 0) {
        return ReldepId();
    }

    if (version) {
        Id evr_id = pool_str2id(pool, version, 1);
        id = pool_rel2id(pool, id, evr_id, static_cast<int>(cmp_type), 1);
    }
    return ReldepId(id);
}

ReldepId Reldep::get_reldep_id(PackageSack * sack, const std::string & reldep_str, int create) {
    if (reldep_str[0] == '(') {
        // Rich dependency
        Pool * pool = sack->p_impl->pool;
        Id id = pool_parserpmrichdep(pool, reldep_str.c_str());
        // TODO(jmracek) Replace runtime_error. Do we need to throw an error?
        if (id == 0) {
            throw std::runtime_error("Cannot parse a dependency string");
        }
        return ReldepId(id);
    }

    solv::ReldepParser dep_splitter;
    if (!dep_splitter.parse(reldep_str)) {
        throw std::runtime_error("Cannot parse a dependency string");
    }
    return get_reldep_id(
        sack, dep_splitter.get_name_cstr(), dep_splitter.get_evr_cstr(), dep_splitter.get_cmp_type(), create);
}

}  // namespace libdnf::rpm
