// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_TRANSACTION_DB_TRANS_WITH_HPP
#define LIBDNF_TRANSACTION_DB_TRANS_WITH_HPP


#include "libdnf/utils/sqlite3/sqlite3.hpp"

#include <set>
#include <string>


namespace libdnf::transaction {


class Transaction;


/// Load records from table 'rpm' associated with a transaction via 'trans_with' table.
/// Return set of package NEVRAs.
std::set<std::string> load_transaction_runtime_packages(libdnf::utils::SQLite3 & conn, Transaction & trans);


/// Insert transaction runtime packages to 'rpm' and 'trans_with' tables.
void save_transaction_runtime_packages(libdnf::utils::SQLite3 & conn, Transaction & trans);


}  // namespace libdnf::transaction


#endif  // LIBDNF_TRANSACTION_DB_TRANS_WITH_HPP
