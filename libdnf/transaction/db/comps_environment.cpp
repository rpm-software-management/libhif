// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "comps_environment.hpp"

#include "comps_environment_group.hpp"
#include "item.hpp"
#include "trans_item.hpp"

#include "libdnf/transaction/comps_group.hpp"
#include "libdnf/transaction/transaction.hpp"
#include "libdnf/transaction/transaction_item.hpp"


namespace libdnf::transaction {


static const char * SQL_COMPS_ENVIRONMENT_TRANSACTION_ITEM_SELECT = R"**(
    SELECT
        /* trans_item */
        ti.id,
        ti.action,
        ti.reason,
        ti.state,
        /* repo */
        r.repoid,
        /* comps_environment */
        i.item_id,
        i.environmentid,
        i.name,
        i.translated_name,
        i.pkg_types
    FROM
        trans_item ti
    JOIN
        repo r ON ti.repo_id == r.id
    JOIN
        comps_environment i USING (item_id)
    WHERE
        ti.trans_id = ?
)**";


std::unique_ptr<libdnf::utils::SQLite3::Query> comps_environment_transaction_item_select_new_query(
    libdnf::utils::SQLite3 & conn, int64_t transaction_id) {
    auto query = std::make_unique<libdnf::utils::SQLite3::Query>(conn, SQL_COMPS_ENVIRONMENT_TRANSACTION_ITEM_SELECT);
    query->bindv(transaction_id);
    return query;
}


std::vector<std::unique_ptr<CompsEnvironment>> get_transaction_comps_environments(
    libdnf::utils::SQLite3 & conn, Transaction & trans) {
    std::vector<std::unique_ptr<CompsEnvironment>> result;

    auto query = comps_environment_transaction_item_select_new_query(conn, trans.get_id());
    while (query->step() == libdnf::utils::SQLite3::Statement::StepResult::ROW) {
        auto ti = std::make_unique<CompsEnvironment>(trans);
        transaction_item_select(*query, *ti);
        ti->set_environment_id(query->get<std::string>("environmentid"));
        ti->set_name(query->get<std::string>("name"));
        ti->set_translated_name(query->get<std::string>("translated_name"));
        ti->set_package_types(static_cast<CompsPackageType>(query->get<int>("pkg_types")));
        comps_environment_groups_select(conn, *ti);
        result.push_back(std::move(ti));
    }

    return result;
}


static const char * SQL_COMPS_ENVIRONMENT_INSERT = R"**(
    INSERT INTO
        comps_environment (
            item_id,
            environmentid,
            name,
            translated_name,
            pkg_types
        )
    VALUES
        (?, ?, ?, ?, ?)
)**";


std::unique_ptr<libdnf::utils::SQLite3::Statement> comps_environment_insert_new_query(libdnf::utils::SQLite3 & conn) {
    auto query = std::make_unique<libdnf::utils::SQLite3::Statement>(conn, SQL_COMPS_ENVIRONMENT_INSERT);
    return query;
}


int64_t comps_environment_insert(libdnf::utils::SQLite3::Statement & query, CompsEnvironment & grp) {
    // insert a record to the 'item' table first
    auto query_item_insert = item_insert_new_query(query.get_db(), TransactionItemType::GROUP);
    auto item_id = item_insert(*query_item_insert);

    query.bindv(
        item_id,
        grp.get_environment_id(),
        grp.get_name(),
        grp.get_translated_name(),
        static_cast<int>(grp.get_package_types()));
    query.step();
    query.reset();
    grp.set_item_id(item_id);
    return item_id;
}


void insert_transaction_comps_environments(libdnf::utils::SQLite3 & conn, Transaction & trans) {
    auto query_comps_environment_insert = comps_environment_insert_new_query(conn);
    auto query_trans_item_insert = trans_item_insert_new_query(conn);

    for (auto & env : trans.get_comps_environments()) {
        comps_environment_insert(*query_comps_environment_insert, *env);
        transaction_item_insert(*query_trans_item_insert, *env);
        comps_environment_groups_insert(conn, *env);
    }
}


}  // namespace libdnf::transaction
