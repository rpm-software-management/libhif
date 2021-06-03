// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "libdnf/transaction/comps_group.hpp"

#include "libdnf/transaction/db/comps_group.hpp"
#include "libdnf/transaction/db/comps_group_package.hpp"
#include "libdnf/transaction/transaction.hpp"
#include "libdnf/transaction/transaction_item.hpp"

#include <algorithm>


namespace libdnf::transaction {


CompsGroup::CompsGroup(Transaction & trans) : TransactionItem::TransactionItem(trans, Type::GROUP) {}


/*
std::vector< TransactionItemPtr >
CompsGroup::getTransactionItemsByPattern(libdnf::utils::SQLite3Ptr conn, const std::string &pattern)
{
    const char *sql = R"**(
        SELECT DISTINCT
            groupid
        FROM
            comps_group
        WHERE
            groupid LIKE ?
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
        auto groupid = query.get< std::string >("groupid");
        auto trans_item = getTransactionItem(conn, groupid);
        if (!trans_item) {
            continue;
        }
        result.push_back(trans_item);
    }
    return result;
}
*/

CompsGroupPackage & CompsGroup::new_package() {
    auto pkg = new CompsGroupPackage(*this);
    auto pkg_ptr = std::unique_ptr<CompsGroupPackage>(std::move(pkg));
    // TODO(dmach): following lines are not thread-safe
    packages.push_back(std::move(pkg_ptr));
    return *packages.back();
}


CompsGroupPackage::CompsGroupPackage(CompsGroup & group) : group(group) {}


}  // namespace libdnf::transaction
