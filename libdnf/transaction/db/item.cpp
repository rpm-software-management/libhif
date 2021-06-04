// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "item.hpp"


namespace libdnf::transaction {


static const char * SQL_ITEM_INSERT = R"**(
    INSERT INTO
        item (
            item_type
        )
    VALUES
        (?)
)**";


std::unique_ptr<libdnf::utils::SQLite3::Statement> item_insert_new_query(
    libdnf::utils::SQLite3 & conn, TransactionItemType type) {
    auto query = std::make_unique<libdnf::utils::SQLite3::Statement>(conn, SQL_ITEM_INSERT);
    query->bindv(static_cast<int>(type));
    return query;
}


int64_t item_insert(libdnf::utils::SQLite3::Statement & query) {
    if (query.step() != libdnf::utils::SQLite3::Statement::StepResult::DONE) {
        // TODO(dmach): replace with a better exception class
        throw std::runtime_error("");
    }
    return query.last_insert_rowid();
}


}  // namespace libdnf::transaction
