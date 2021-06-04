// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "repo.hpp"


namespace libdnf::transaction {


static const char * SQL_REPO_INSERT = R"**(
    INSERT INTO
        repo (
            repoid
        )
    VALUES
        (?)
)**";


std::unique_ptr<libdnf::utils::SQLite3::Statement> repo_insert_new_query(libdnf::utils::SQLite3 & conn) {
    auto query = std::make_unique<libdnf::utils::SQLite3::Statement>(conn, SQL_REPO_INSERT);
    return query;
}


int64_t repo_insert(libdnf::utils::SQLite3::Statement & query, const std::string & repoid) {
    query.bindv(repoid);
    query.step();
    int64_t result = query.last_insert_rowid();
    query.reset();
    return result;
}


static const char * SQL_REPO_SELECT_PK = R"**(
    SELECT
        id
    FROM
        repo
    WHERE
        repoid = ?
)**";


std::unique_ptr<libdnf::utils::SQLite3::Statement> repo_select_pk_new_query(libdnf::utils::SQLite3 & conn) {
    auto query = std::make_unique<libdnf::utils::SQLite3::Statement>(conn, SQL_REPO_SELECT_PK);
    return query;
}


int64_t repo_select_pk(libdnf::utils::SQLite3::Statement & query, const std::string & repoid) {
    query.bindv(repoid);

    int64_t result = 0;
    if (query.step() == libdnf::utils::SQLite3::Statement::StepResult::ROW) {
        result = query.get<int64_t>(0);
    }
    query.reset();
    return result;
}


}  // namespace libdnf::transaction
