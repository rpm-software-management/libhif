// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_RPM_CHECKSUM_HPP
#define LIBDNF_RPM_CHECKSUM_HPP

#include <string>

namespace libdnf::rpm {

// forward declarations
class Package;

/// Class contains checksum and checksum type
class Checksum {
public:
    enum class Type { UNKNOWN, MD5, SHA1, SHA224, SHA256, SHA384, SHA512 };
    Type get_type() const noexcept;
    std::string get_type_str() const;

    /// Return checksum in hex format
    const std::string & get_checksum() const noexcept { return checksum; };


private:
    friend class Package;

    /// Require checksum in hex and libsolv checksum type
    Checksum(const char * checksum, int libsolv_type);

    std::string checksum;
    int libsolv_type;
};


/// Constructor requires checksum in hex and libsolv checksum type
inline Checksum::Checksum(const char * checksum, int libsolv_type) : libsolv_type(libsolv_type) {
    if (checksum != nullptr) {
        this->checksum = checksum;
    }
}


}  // namespace libdnf::rpm

#endif  // LIBDNF_RPM_CHECKSUM_HPP
