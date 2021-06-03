// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_TRANSACTION_DB_TRANS_ITEM_HPP
#define LIBDNF_TRANSACTION_DB_TRANS_ITEM_HPP


#include "libdnf/utils/sqlite3/sqlite3.hpp"

#include <memory>


namespace libdnf::transaction {


class TransactionItem;


/// Copy 'trans_item' fields from a query to TransactionItem or an object that inherits from it
void transaction_item_select(libdnf::utils::SQLite3::Query & query, TransactionItem & ti);


/// Create a query (statement) that inserts new records to the 'trans_item' table
std::unique_ptr<libdnf::utils::SQLite3::Statement> trans_item_insert_new_query(libdnf::utils::SQLite3 & conn);


/// Use a query to insert a new record to the 'trans_item' table
int64_t transaction_item_insert(libdnf::utils::SQLite3::Statement & query, TransactionItem & ti);


}  // namespace libdnf::transaction


#endif  // LIBDNF_TRANSACTION_DB_TRANS_ITEM_HPP
