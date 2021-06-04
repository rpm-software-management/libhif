// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_TRANSACTION_DB_TRANS_HPP
#define LIBDNF_TRANSACTION_DB_TRANS_HPP


#include "libdnf/utils/sqlite3/sqlite3.hpp"

#include <memory>


namespace libdnf::transaction {


class Transaction;


/// Create a query that returns a record from 'trans' table
std::unique_ptr<libdnf::utils::SQLite3::Query> trans_select_new_query(libdnf::utils::SQLite3 & conn);


/// Use a query to retrieve data from the 'trans' table and store it in a Transaction object
bool trans_select(libdnf::utils::SQLite3::Query & query, int64_t transaction_id, Transaction & trans);


/// Create a query for inserting records to the 'trans' table
std::unique_ptr<libdnf::utils::SQLite3::Statement> trans_insert_new_query(libdnf::utils::SQLite3 & conn);


/// Use a query to insert a new record to the 'trans' table
void trans_insert(libdnf::utils::SQLite3::Statement & query, Transaction & trans);


/// Create a query that updates a record in 'trans' table
std::unique_ptr<libdnf::utils::SQLite3::Statement> trans_update_new_query(libdnf::utils::SQLite3 & conn);


/// Use a query to update a record in the 'trans' table
void trans_update(libdnf::utils::SQLite3::Statement & query, const Transaction & trans);


}  // namespace libdnf::transaction


#endif  // LIBDNF_TRANSACTION_DB_TRANS_HPP
