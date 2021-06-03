// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "libdnf/rpm/checksum.hpp"

extern "C" {
#include <solv/chksum.h>
}

namespace libdnf::rpm {

std::string Checksum::get_type_str() const {
    auto type_str = solv_chksum_type2str(libsolv_type);
    return type_str ? type_str : "unknown";
}

Checksum::Type Checksum::get_type() const noexcept {
    switch (libsolv_type) {
        case REPOKEY_TYPE_MD5:
            return Type::MD5;
        case REPOKEY_TYPE_SHA1:
            return Type::SHA1;
        case REPOKEY_TYPE_SHA224:
            return Type::SHA224;
        case REPOKEY_TYPE_SHA256:
            return Type::SHA256;
        case REPOKEY_TYPE_SHA384:
            return Type::SHA384;
        case REPOKEY_TYPE_SHA512:
            return Type::SHA512;
        default:
            return Type::UNKNOWN;
    }
}

}  //  namespace libdnf::rpm
