// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_ADVISORY_ADVISORY_REFERENCE_HPP
#define LIBDNF_ADVISORY_ADVISORY_REFERENCE_HPP

#include "advisory.hpp"

#include <memory>
#include <vector>


namespace libdnf::advisory {

//TODO(amatej): add unit tests for AdvisoryReference
class AdvisoryReference {
public:
    using Type = AdvisoryReferenceType;

    /// Get id of this advisory reference, this id is like a name of this reference
    /// (it is not libsolv id).
    ///
    /// @return id of this reference as std::string.
    std::string get_id() const;

    /// Get type of this reference.
    ///
    /// @return Type of this reference.
    Type get_type() const;

    /// Get title of this reference.
    ///
    /// @return Title of this reference.
    std::string get_title() const;

    /// Get url of this reference.
    ///
    /// @return Url of this reference.
    std::string get_url() const;

private:
    friend class Advisory;

    /// Construct AdvisoryReference
    ///
    /// @param base     Reference to Base instance.
    /// @param advisory AdvisoryId into libsolv pool.
    /// @param index    Index of this reference in its advisory.
    /// @return New AdvisoryReference instance.
    AdvisoryReference(const libdnf::rpm::PackageSackWeakPtr & sack, AdvisoryId advisory, int index);

    libdnf::rpm::PackageSackWeakPtr sack;
    AdvisoryId advisory;

    /// We cannot store IDs of reference data (id, type, title, url) because they
    /// don't have ids set in libsolv (they are only strings), therefore we store
    /// index of the reference.
    int index;
};

}  // namespace libdnf::advisory

#endif
