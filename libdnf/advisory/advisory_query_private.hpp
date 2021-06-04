// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_ADVISORY_ADVISORY_QUERY_PRIVATE_HPP
#define LIBDNF_ADVISORY_ADVISORY_QUERY_PRIVATE_HPP

#include "libdnf/advisory/advisory.hpp"
#include "libdnf/advisory/advisory_query.hpp"
#include "libdnf/rpm/solv/solv_map.hpp"

namespace libdnf::advisory {

//TODO(amatej): only temprary class to hide solv_map from advisory_query.hpp,
//              should be replaced most likely by AdvisorySet?
class AdvisoryQuery::Impl {
    Impl(libdnf::rpm::solv::SolvMap m) : data_map(m){};

private:
    friend AdvisoryQuery;

    libdnf::rpm::solv::SolvMap data_map;
};

}  // namespace libdnf::advisory


#endif  // LIBDNF_ADVISORY_ADVISORY_QUERY_PRIVATE_HPP
