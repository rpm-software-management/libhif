// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_TRANSACTION_DB_RPM_HPP
#define LIBDNF_TRANSACTION_DB_RPM_HPP


#include "libdnf/utils/sqlite3/sqlite3.hpp"

#include <memory>
#include <vector>


namespace libdnf::transaction {


class Package;
class Transaction;
class TransactionItem;


/// Create a query that returns all rpm transaction items for a transaction
std::unique_ptr<libdnf::utils::SQLite3::Query> rpm_transaction_item_select_new_query(
    libdnf::utils::SQLite3 & conn, int64_t transaction_id);


/// Use a query to select a record from the 'rpm' table and populate a TransactionItem
int64_t rpm_transaction_item_select(libdnf::utils::SQLite3::Query & query, Package & pkg);


/// Create a query (statement) that inserts new records to the 'rpm' table
std::unique_ptr<libdnf::utils::SQLite3::Statement> rpm_insert_new_query(libdnf::utils::SQLite3 & conn);


/// Use a query to insert a new record to the 'rpm' table
int64_t rpm_insert(libdnf::utils::SQLite3::Statement & query, const Package & rpm);


/// Create a query that returns primary keys from table 'rpm'
std::unique_ptr<libdnf::utils::SQLite3::Statement> rpm_select_pk_new_query(libdnf::utils::SQLite3 & conn);


/// Find a primary key of a recod in table 'rpm' that matches the Package.
/// Return an existing primary key or 0 if the record was not found.
int64_t rpm_select_pk(libdnf::utils::SQLite3::Statement & query, const Package & rpm);


/// Create a query to select a record from the 'rpm' table and populate a Package
std::unique_ptr<libdnf::utils::SQLite3::Query> rpm_select_new_query(libdnf::utils::SQLite3 & conn);


/// Use a query to select a record from 'rpm' table and populate a Package
bool rpm_select(libdnf::utils::SQLite3::Query & query, int64_t rpm_id, Package & rpm);


/// Return a vector of Package objects with packages in a transaction
std::vector<std::unique_ptr<Package>> get_transaction_packages(libdnf::utils::SQLite3 & conn, Transaction & trans);


/// Insert Package objects associated with a transaction into the database
void insert_transaction_packages(libdnf::utils::SQLite3 & conn, Transaction & trans);


}  // namespace libdnf::transaction


#endif  // LIBDNF_TRANSACTION_DB_RPM_HPP
