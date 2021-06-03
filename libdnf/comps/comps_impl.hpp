// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_COMPS_COMPS_IMPL_HPP
#define LIBDNF_COMPS_COMPS_IMPL_HPP

#include "libdnf/comps/comps.hpp"

extern "C" {
#include <solv/pool.h>
}


namespace libdnf::comps {


class Comps::Impl {
public:
    explicit Impl();
    ~Impl();

    Pool * get_pool() { return pool; }

private:
    friend class Comps;

    WeakPtrGuard<Comps, false> data_guard;
    Pool * pool;
};


}  // namespace libdnf::comps


#endif  // LIBDNF_COMPS_COMPS_IMPL_HPP
