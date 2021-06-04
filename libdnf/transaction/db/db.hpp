// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_TRANSACTION_DB_DB_HPP
#define LIBDNF_TRANSACTION_DB_DB_HPP


#include "libdnf/utils/sqlite3/sqlite3.hpp"

#include <memory>


namespace libdnf {
class Base;
}


namespace libdnf::transaction {


/// Create tables and migrate schema if necessary.
void transaction_db_create(libdnf::utils::SQLite3 & conn);


/// Create a connection to transaction database in the 'persistdir' directory.
/// The file is named 'history.sqlite' for compatibility reasons.
std::unique_ptr<libdnf::utils::SQLite3> transaction_db_connect(libdnf::Base & base);


}  // namespace libdnf::transaction


#endif  // LIBDNF_TRANSACTION_DB_DB_HPP
