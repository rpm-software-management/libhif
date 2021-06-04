// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_TRANSACTION_DB_COMPS_GROUP_PACKAGE_HPP
#define LIBDNF_TRANSACTION_DB_COMPS_GROUP_PACKAGE_HPP


#include "libdnf/transaction/comps_group.hpp"
#include "libdnf/utils/sqlite3/sqlite3.hpp"


namespace libdnf::transaction {


/// Load GroupPackage objects from the database to the CompsGroup object
void comps_group_packages_select(libdnf::utils::SQLite3 & conn, CompsGroup & group);


/// Insert GroupPackage objects associated with a CompsGroup into the database
void comps_group_packages_insert(libdnf::utils::SQLite3 & conn, CompsGroup & group);


}  // namespace libdnf::transaction


#endif  // LIBDNF_TRANSACTION_DB_COMPS_GROUP_PACKAGE_HPP
