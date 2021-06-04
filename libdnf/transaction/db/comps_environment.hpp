// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_TRANSACTION_DB_COMPS_ENVIRONMENT_HPP
#define LIBDNF_TRANSACTION_DB_COMPS_ENVIRONMENT_HPP


#include "libdnf/utils/sqlite3/sqlite3.hpp"

#include <memory>


namespace libdnf::transaction {


class CompsEnvironment;
class Transaction;
class TransactionItem;


/// Return a vector of CompsEnvironment objects with comps environments in a transaction
std::vector<std::unique_ptr<CompsEnvironment>> get_transaction_comps_environments(
    libdnf::utils::SQLite3 & conn, Transaction & trans);


/// Create a query (statement) that inserts new records to the 'comps_environment' table
std::unique_ptr<libdnf::utils::SQLite3::Statement> comps_environment_insert_new_query(libdnf::utils::SQLite3 & conn);


/// Use a query to insert a new record to the 'comps_environment' table
int64_t comps_environment_insert(libdnf::utils::SQLite3::Statement & query, CompsEnvironment & env);


/// Insert CompsEnvironment objects associated with a transaction into the database
void insert_transaction_comps_environments(libdnf::utils::SQLite3 & conn, Transaction & trans);


}  // namespace libdnf::transaction


#endif  // LIBDNF_TRANSACTION_DB_COMPS_ENVIRONMENT_HPP
