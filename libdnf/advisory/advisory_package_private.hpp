// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_ADVISORY_ADVISORY_PACKAGE_PRIVATE_HPP
#define LIBDNF_ADVISORY_ADVISORY_PACKAGE_PRIVATE_HPP

#include "libdnf/advisory/advisory_package.hpp"
#include "libdnf/rpm/package_sack_impl.hpp"
#include "libdnf/rpm/solv/package_private.hpp"

#include <solv/pooltypes.h>
#include <solv/solvable.h>

namespace libdnf::advisory {

//TODO(amatej): We might want to remove the Impl idiom to speed up iterating over
//              AdvisoryPackages (less classes, less overhead), but we would end
//              up with libsolv ints (Ids) in a public header.
class AdvisoryPackage::Impl {
public:
    /// Copy constructor: clone from an existing AdvisoryPackage::Impl
    Impl(const Impl & other);
    /// Move constructor: clone from an existing AdvisoryPackage::Impl
    Impl(Impl && other);

    Impl & operator=(const Impl & other);
    Impl & operator=(Impl && other);

    std::string get_name() const;
    std::string get_version() const;
    std::string get_evr() const;
    std::string get_arch() const;
    Id get_name_id() const { return name; };
    Id get_evr_id() const { return evr; };
    Id get_arch_id() const { return arch; };
    AdvisoryId get_advisory_id() const { return advisory; };

    //TODO(amatej): Is this the correct name?
    /// Compare NEVRAs of two AdvisoryPackages
    ///
    /// @param first        First AdvisoryPackage to compare.
    /// @param second       Secondf AdvisoryPackage to compare.
    /// @return True if first AdvisoryPackage has smaller NEVRA, False otherwise.
    static bool nevra_compare_lower_id(const AdvisoryPackage & first, const AdvisoryPackage & second) {
        if (first.p_impl->name != second.p_impl->name)
            return first.p_impl->name < second.p_impl->name;
        if (first.p_impl->arch != second.p_impl->arch)
            return first.p_impl->arch < second.p_impl->arch;
        return first.p_impl->evr < second.p_impl->evr;
    }

    //TODO(amatej): Is this the correct name?
    /// Compare Name and Architecture of AdvisoryPackage and libdnf::rpm::Package
    ///
    /// @param adv_pkg      AdvisoryPackage to compare.
    /// @param pkg          libdnf::rpm::Package to compare.
    /// @return True if AdvisoryPackage has smaller name or architecture than libdnf::rpm::package, False otherwise.
    static bool name_arch_compare_lower_id(const AdvisoryPackage & adv_pkg, const rpm::Package & pkg) {
        Pool * pool = adv_pkg.p_impl->sack->p_impl->get_pool();
        Solvable * s = libdnf::rpm::solv::get_solvable(pool, pkg.get_id().id);

        if (adv_pkg.p_impl->name != s->name)
            return adv_pkg.p_impl->name < s->name;
        return adv_pkg.p_impl->arch < s->arch;
    }

    //TODO(amatej): Is this the correct name?
    /// Compare Name and Architecture of Solvable and AdvisoryPackage::Impl
    ///
    /// @param solvable     Solvable to compare
    /// @param adv_pkg      AdvisoryPackage::Impl to compare.
    /// @return True if Solvable has smaller name or architecture than AdvisoryPackage::Impl, False otherwise.
    static bool name_arch_compare_lower_solvable(const Solvable * solvable, const AdvisoryPackage::Impl & adv_pkg) {
        if (solvable->name != adv_pkg.name) {
            return solvable->name < adv_pkg.name;
        }
        return solvable->arch < adv_pkg.arch;
    }

    //TODO(amatej): Is this the correct name?
    /// Compare NEVRAs of Solvable and AdvisoryPackage::Impl
    ///
    /// @param solvable     Solvable to compare
    /// @param adv_pkg      AdvisoryPackage::Impl to compare.
    /// @return True if Solvable has smaller nevra than AdvisoryPackage::Impl, False otherwise.
    static bool nevra_compare_lower_solvable(const Solvable * solvable, const AdvisoryPackage::Impl & adv_pkg) {
        if (solvable->name != adv_pkg.name) {
            return solvable->name < adv_pkg.name;
        }
        if (solvable->arch != adv_pkg.arch) {
            return solvable->arch < adv_pkg.arch;
        }
        return solvable->evr < adv_pkg.evr;
    }

private:
    friend class AdvisoryCollection;
    friend AdvisoryPackage;

    Impl(
        libdnf::rpm::PackageSack & sack,
        AdvisoryId advisory,
        int owner_collection_index,
        Id name,
        Id evr,
        Id arch,
        const char * filename);

    AdvisoryId advisory;
    int owner_collection_index;

    Id name;
    Id evr;
    Id arch;
    const char * filename;
    libdnf::rpm::PackageSackWeakPtr sack;
};

}  // namespace libdnf::advisory


#endif  // LIBDNF_ADVISORY_ADVISORY_PACKAGE_PRIVATE_HPP
