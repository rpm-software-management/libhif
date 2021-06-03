// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_RPM_SOLV_SOLV_PRIVATE_HPP
#define LIBDNF_RPM_SOLV_SOLV_PRIVATE_HPP


#include "../package_sack_impl.hpp"
#include "../../repo/repo_impl.hpp"

namespace libdnf::rpm::solv {

class SolvPrivate {
public:
    static void internalize_libsolv_repo(repo::LibsolvRepo * libsolv_repo);

private:
};

inline void SolvPrivate::internalize_libsolv_repo(repo::LibsolvRepo * libsolv_repo) {
    PackageSack::Impl::internalize_libsolv_repo(libsolv_repo);
}

}  // namespace libdnf::rpm::solv

#endif  // LIBDNF_RPM_SOLV_SOLV_PRIVATE_HPP
