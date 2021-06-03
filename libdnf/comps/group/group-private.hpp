// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_COMPS_GROUP_GROUP_PRIVATE_HPP
#define LIBDNF_COMPS_GROUP_GROUP_PRIVATE_HPP


#include <libdnf/comps/group/group.hpp>

extern "C" {
#include <solv/pool.h>
}

namespace libdnf::comps {

    
void add_solvable_id(Group & group, Id solvable_id);


void add_solvable_ids(Group & group, std::vector<Id> solvable_ids);


// Search solvables that correspond to the group_ids for given key
// Return first non-empty string
std::string lookup_str(Pool * pool, std::vector<GroupId> group_ids, Id key);


}  // namespace libdnf::comps

#endif  // LIBDNF_COMPS_GROUP_GROUP_PRIVATE_HPP
