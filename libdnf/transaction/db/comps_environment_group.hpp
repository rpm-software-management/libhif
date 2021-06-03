// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_TRANSACTION_DB_COMPS_ENVIRONMENT_GROUP_HPP
#define LIBDNF_TRANSACTION_DB_COMPS_ENVIRONMENT_GROUP_HPP


#include "libdnf/transaction/comps_environment.hpp"
#include "libdnf/utils/sqlite3/sqlite3.hpp"


namespace libdnf::transaction {


/// Load EnvironmentGroup objects from the database to the CompsEnvironment object
void comps_environment_groups_select(libdnf::utils::SQLite3 & conn, CompsEnvironment & env);


/// Insert EnvironmentGroup objects associated with a CompsEnvironment into the database
void comps_environment_groups_insert(libdnf::utils::SQLite3 & conn, CompsEnvironment & env);


}  // namespace libdnf::transaction


#endif  // LIBDNF_TRANSACTION_DB_COMPS_ENVIRONMENT_GROUP_HPP
