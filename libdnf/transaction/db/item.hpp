// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_TRANSACTION_DB_ITEM_HPP
#define LIBDNF_TRANSACTION_DB_ITEM_HPP


#include "libdnf/transaction/transaction_item_type.hpp"
#include "libdnf/utils/sqlite3/sqlite3.hpp"

#include <memory>


namespace libdnf::transaction {


/// Create a query (statement) that inserts new records to the 'item' table
std::unique_ptr<libdnf::utils::SQLite3::Statement> item_insert_new_query(
    libdnf::utils::SQLite3 & conn, TransactionItemType type);


/// Use a query to insert a new record to the 'item' table
int64_t item_insert(libdnf::utils::SQLite3::Statement & query);


}  // namespace libdnf::transaction


#endif  // LIBDNF_TRANSACTION_DB_ITEM_HPP
