// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_TRANSACTION_DB_REPO_HPP
#define LIBDNF_TRANSACTION_DB_REPO_HPP


#include "libdnf/utils/sqlite3/sqlite3.hpp"

#include <memory>
#include <string>


namespace libdnf::transaction {


/// Create a query (statement) that inserts new records to the 'repo' table
std::unique_ptr<libdnf::utils::SQLite3::Statement> repo_insert_new_query(libdnf::utils::SQLite3 & conn);


/// Use a query to insert a new record to the 'repo' table
int64_t repo_insert(libdnf::utils::SQLite3::Statement & query, const std::string & repoid);


/// Create a query that returns primary keys from table 'repo'
std::unique_ptr<libdnf::utils::SQLite3::Statement> repo_select_pk_new_query(libdnf::utils::SQLite3 & conn);


/// Find a primary key of a recod in table 'repo' that matches the Package.
/// Return an existing primary key or 0 if the record was not found.
int64_t repo_select_pk(libdnf::utils::SQLite3::Statement & query, const std::string & repoid);


}  // namespace libdnf::transaction


#endif  // LIBDNF_TRANSACTION_DB_REPO_HPP
