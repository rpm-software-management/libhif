/*
Copyright (C) 2021 Red Hat, Inc.

This file is part of libdnf: https://github.com/rpm-software-management/libdnf/

Libdnf is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Libdnf is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with libdnf.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef LIBDNF_ADVISORY_ADVISORY_PACKAGE_HPP
#define LIBDNF_ADVISORY_ADVISORY_PACKAGE_HPP

#include "libdnf/advisory/advisory.hpp"

#include <memory>
#include <vector>


namespace libdnf::rpm {

class PackageQuery;

}

namespace libdnf::advisory {

class AdvisoryPackage {
public:
    AdvisoryPackage(const AdvisoryPackage & src);
    AdvisoryPackage(AdvisoryPackage && src);

    AdvisoryPackage & operator=(const AdvisoryPackage & src);
    AdvisoryPackage & operator=(AdvisoryPackage && src) noexcept;
    ~AdvisoryPackage();

    /// Get name of this AdvisoryPackage.
    ///
    /// @return Name of this AdvisoryPackage as std::string.
    std::string get_name() const;

    /// Get version of this AdvisoryPackage.
    ///
    /// @return Version of this AdvisoryPackage as std::string.
    std::string get_version() const;

    /// Get evr of this AdvisoryPackage.
    ///
    /// @return Evr of this AdvisoryPackage as std::string.
    std::string get_evr() const;

    /// Get arch of this AdvisoryPackage.
    ///
    /// @return Arch of this AdvisoryPackage as std::string.
    std::string get_arch() const;

    /// Get AdvisoryId of Advisory this AdvisoryPackage belongs to.
    ///
    /// @return AdvisoryId of this AdvisoryPackage.
    AdvisoryId get_advisory_id() const;

    //TODO(amatej): we should be able to get Advisory and AdvisoryCollection information
    //              from a package, returning new advisory object is one option but it might
    //              be better to set up so sort of inheritance among Advisory, AdvisoryCollection,
    //              AdvisoryPacakge and AdvisoryModule.
    /// Get Advisory this AdvisoryPackage belongs to.
    ///
    /// @return newly construted Advisory object of this AdvisoryPackage.
    Advisory get_advisory() const;
    /// Get AdvisoryCollection this AdvisoryPackage belongs to.
    ///
    /// @return newly construted AdvisoryCollection object of this AdvisoryPackage.
    AdvisoryCollection get_advisory_collection() const;

private:
    friend class AdvisoryCollection;
    friend class AdvisoryQuery;
    friend class libdnf::rpm::PackageQuery;

    class Impl;
    AdvisoryPackage(Impl * private_pkg);
    std::unique_ptr<Impl> p_impl;
};

}  // namespace libdnf::advisory

#endif
