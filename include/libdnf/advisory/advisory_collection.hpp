// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_ADVISORY_ADVISORY_COLLECTION_HPP
#define LIBDNF_ADVISORY_ADVISORY_COLLECTION_HPP

#include "advisory.hpp"
#include "advisory_module.hpp"
#include "advisory_package.hpp"

#include <vector>


namespace libdnf::advisory {

//TODO(amatej): add unit tests for AdvisoryCollection
class AdvisoryCollection {
public:
    /// Whether this AdvisoryCollection is applicable. True when at least one AdvisoryModule in this
    /// AdvisoryCollection is active on the system, False otherwise.
    bool is_applicable() const;

    /// Get AdvisoryId of Advisory this AdvisoryCollection belongs to.
    ///
    /// @return AdvisoryId of this AdvisoryCollection.
    AdvisoryId get_advisory_id() const;

    /// Get all AdvisoryPackages stored in this AdvisoryCollection
    ///
    /// @return std::vector of AdvisorPackages used as output.
    std::vector<AdvisoryPackage> get_packages();

    /// Get all AdvisoryModules stored in this AdvisoryCollection
    ///
    /// @return std::vector of AdvisorModules.
    std::vector<AdvisoryModule> get_modules();

    //TODO(amatej): we should be able to get advisory information from a collection,
    //              returning new advisory object is one option but it might be better
    //              to set up so sort of inheritance among Advisory, AdvisoryCollection
    //              and possibly AdvisoryPacakge, AdvisoryModule.
    /// Get Advisory this AdvisoryCollection belongs to.
    ///
    /// @return newly construted Advisory object of this AdvisoryCollection.
    Advisory get_advisory() const;

private:
    friend Advisory;
    friend AdvisoryQuery;
    friend AdvisoryPackage;
    friend AdvisoryModule;

    AdvisoryCollection(const libdnf::rpm::PackageSackWeakPtr & sack, AdvisoryId advisory, int index);

    //TODO(amatej): Hide into an Impl?
    /// Get all AdvisoryPackages stored in this AdvisoryCollection
    ///
    /// @param output           std::vector of AdvisorPackages used as output.
    ///                         This is much faster than returning new std::vector and later joining
    ///                         them when collecting AdvisoryPackages from multiple collections.
    /// @param with_filenames   Filenames of AdvisoryPackages are not always useful, this allows skipping them.
    ///                         The filename is stored as a c string (not libsolv id) this incurs slowdown.
    void get_packages(std::vector<AdvisoryPackage> & output, bool with_filenames = false);

    //TODO(amatej): Hide into an Impl?
    /// Get all AdvisoryModules stored in this AdvisoryCollection
    /// @param output           std::vector of AdvisorModules used as output.
    ///                         This is much faster than returning new std::vector and later joining
    ///                         them when collecting AdvisoryModules from multiple collections.
    void get_modules(std::vector<AdvisoryModule> & output);

    libdnf::rpm::PackageSackWeakPtr sack;
    AdvisoryId advisory;

    /// AdvisoryCollections don't have their own Id, therefore store it's index in its Advisory (just like AdvisoryReference)
    int index;
};

}  // namespace libdnf::advisory

#endif
