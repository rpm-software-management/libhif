// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_COMPS_COMPS_HPP
#define LIBDNF_COMPS_COMPS_HPP

#include "libdnf/common/weak_ptr.hpp"
#include "libdnf/comps/group/sack.hpp"

#include <memory>


namespace libdnf {
class Base;
}


namespace libdnf::comps {

using CompsWeakPtr = libdnf::WeakPtr<Comps, false>;

class Comps {
public:
    explicit Comps(libdnf::Base & base);
    ~Comps();

    void load_installed();
    // Load comps from given file into the pool
    void load_from_file(const std::string & path, const char * reponame);
    GroupSack & get_group_sack() { return group_sack; }

    CompsWeakPtr get_weak_ptr();

private:
    libdnf::Base & base;
    GroupSack group_sack{*this};

    class Impl;
    std::unique_ptr<Impl> p_impl;

    friend GroupSack;
    friend GroupQuery;
    friend Group;
};


}  // namespace libdnf::comps


#endif  // LIBDNF_COMPS_COMPS_HPP
