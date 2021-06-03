// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "libdnf/transaction/comps_environment.hpp"

#include "libdnf/transaction/db/comps_environment.hpp"
#include "libdnf/transaction/db/comps_environment_group.hpp"
#include "libdnf/transaction/transaction.hpp"
#include "libdnf/transaction/transaction_item.hpp"


namespace libdnf::transaction {


CompsEnvironment::CompsEnvironment(Transaction & trans) : TransactionItem::TransactionItem(trans, Type::ENVIRONMENT) {}


/*
std::vector< TransactionItemPtr >
CompsEnvironmentItem::getTransactionItemsByPattern(libdnf::utils::SQLite3Ptr conn, const std::string &pattern)
{
    string sql = R"**(
            SELECT DISTINCT
                environmentid
            FROM
                comps_environment
            WHERE
                environmentid LIKE ?
                OR name LIKE ?
                OR translated_name LIKE ?
        )**";

    std::vector< TransactionItemPtr > result;

    // HACK: create a private connection to avoid undefined behavior
    // after forking process in Anaconda
    libdnf::utils::SQLite3 privateConn(conn->get_path());
    libdnf::utils::SQLite3::Query query(privateConn, sql);
    std::string pattern_sql = pattern;
    std::replace(pattern_sql.begin(), pattern_sql.end(), '*', '%');
    query.bindv(pattern, pattern, pattern);
    while (query.step() == libdnf::utils::SQLite3::Statement::StepResult::ROW) {
        auto groupid = query.get< std::string >("environmentid");
        auto trans_item = getTransactionItem(conn, groupid);
        if (!trans_item) {
            continue;
        }
        result.push_back(trans_item);
    }
    return result;
}
*/


CompsEnvironmentGroup::CompsEnvironmentGroup(CompsEnvironment & environment) : environment(environment) {}


CompsEnvironmentGroup & CompsEnvironment::new_group() {
    auto grp = new CompsEnvironmentGroup(*this);
    auto grp_ptr = std::unique_ptr<CompsEnvironmentGroup>(grp);
    // TODO(dmach): following lines are not thread-safe
    groups.push_back(std::move(grp_ptr));
    return *groups.back();
}


}  // namespace libdnf::transaction
