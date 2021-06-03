// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_TRANSACTION_DB_CONSOLE_OUTPUT_HPP
#define LIBDNF_TRANSACTION_DB_CONSOLE_OUTPUT_HPP


#include "libdnf/utils/sqlite3/sqlite3.hpp"

#include <memory>
#include <string>


namespace libdnf::transaction {


class Transaction;


/// Insert file_descriptor and line into the 'console_output' table.
int64_t console_output_insert_line(
    libdnf::utils::SQLite3 & conn, Transaction & trans, int file_descriptor, const std::string & line);


/// Load records from the 'console_output' table associated with a transaction.
/// Return vector of <file_descriptor, line> pairs.
std::vector<std::pair<int, std::string>> console_output_load(libdnf::utils::SQLite3 & conn, Transaction & trans);


}  // namespace libdnf::transaction


#endif  // LIBDNF_TRANSACTION_DB_CONSOLE_OUTPUT_HPP
