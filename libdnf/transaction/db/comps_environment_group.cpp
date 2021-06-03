// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "comps_environment_group.hpp"

#include "libdnf/transaction/transaction.hpp"

#include <algorithm>
#include <memory>


namespace libdnf::transaction {


static const char * SQL_COMPS_ENVIRONMENT_GROUP_SELECT = R"**(
    SELECT
        id,
        groupid,
        installed,
        group_type
    FROM
        comps_environment_group
    WHERE
        environment_id = ?
    ORDER BY
        id
)**";


std::unique_ptr<libdnf::utils::SQLite3::Query> comps_environment_group_select_new_query(libdnf::utils::SQLite3 & conn) {
    auto query = std::make_unique<libdnf::utils::SQLite3::Query>(conn, SQL_COMPS_ENVIRONMENT_GROUP_SELECT);
    return query;
}


void comps_environment_groups_select(libdnf::utils::SQLite3 & conn, CompsEnvironment & env) {
    auto query = comps_environment_group_select_new_query(conn);
    query->bindv(env.get_item_id());

    while (query->step() == libdnf::utils::SQLite3::Statement::StepResult::ROW) {
        auto & grp = env.new_group();
        grp.set_id(query->get<int64_t>("id"));
        grp.set_group_id(query->get<std::string>("groupid"));
        grp.set_installed(query->get<bool>("installed"));
        grp.set_group_type(static_cast<CompsPackageType>(query->get<int>("group_type")));
    }
}


static const char * SQL_COMPS_ENVIRONMENT_GROUP_INSERT = R"**(
    INSERT INTO
        comps_environment_group (
            environment_id,
            groupid,
            installed,
            group_type
        )
    VALUES
        (?, ?, ?, ?)
)**";


std::unique_ptr<libdnf::utils::SQLite3::Statement> comps_environment_group_insert_new_query(
    libdnf::utils::SQLite3 & conn) {
    auto query = std::make_unique<libdnf::utils::SQLite3::Statement>(conn, SQL_COMPS_ENVIRONMENT_GROUP_INSERT);
    return query;
}


void comps_environment_groups_insert(libdnf::utils::SQLite3 & conn, CompsEnvironment & env) {
    auto query = comps_environment_group_insert_new_query(conn);

    for (auto & grp : env.get_groups()) {
        query->bindv(
            env.get_item_id(), grp->get_group_id(), grp->get_installed(), static_cast<int>(grp->get_group_type()));
        if (query->step() != libdnf::utils::SQLite3::Statement::StepResult::DONE) {
            // TODO(dmach): replace with a better exception class
            throw std::runtime_error("");
        }
        grp->set_id(query->last_insert_rowid());
        query->reset();
    }
}


}  // namespace libdnf::transaction
