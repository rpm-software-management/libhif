// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_ADVISORY_ADVISORY_MODULE_PRIVATE_HPP
#define LIBDNF_ADVISORY_ADVISORY_MODULE_PRIVATE_HPP

#include "libdnf/advisory/advisory.hpp"
#include "libdnf/advisory/advisory_module.hpp"

#include <solv/pooltypes.h>

namespace libdnf::advisory {

//TODO(amatej): We might want to remove the Impl idiom to speed up iterating over
//              AdvisoryModules (less classes, less overhead), but we would end
//              up with libsolv ints (Ids) in a public header.
class AdvisoryModule::Impl {
public:
    /// Copy constructor: clone from an existing AdvisoryModule::Impl
    Impl(const Impl & other);
    /// Move constructor: clone from an existing AdvisoryModule::Impl
    Impl(Impl && other);

    Impl & operator=(const Impl & other);
    Impl & operator=(Impl && other);

private:
    friend class AdvisoryCollection;
    friend AdvisoryModule;

    explicit Impl(
        libdnf::rpm::PackageSack & sack,
        AdvisoryId advisory,
        int owner_collection_index,
        Id name,
        Id stream,
        Id version,
        Id context,
        Id arch);

    libdnf::rpm::PackageSackWeakPtr sack;
    AdvisoryId advisory;
    int owner_collection_index;

    Id name;
    Id stream;
    Id version;
    Id context;
    Id arch;
};

}  // namespace libdnf::advisory


#endif  // LIBDNF_ADVISORY_ADVISORY_MODULE_PRIVATE_HPP
