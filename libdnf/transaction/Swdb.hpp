// TODO(dmach): probably remove most of the functionality and/or merge it into TransactionSack
// the whole file is disabled via the SKIP macro because it doesn't compile with the new code
#ifdef SKIP

/*
 * Copyright (C) 2017-2018 Red Hat, Inc.
 *
 * Licensed under the GNU Lesser General Public License Version 2.1
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef LIBDNF_TRANSACTION_SWDB_HPP
#define LIBDNF_TRANSACTION_SWDB_HPP

#include <map>
#include <memory>
#include <solv/pooltypes.h>
#include <sys/stat.h>
#include <vector>

namespace libdnf::transaction {
struct Swdb;
//class Transformer;
}

//#include "../hy-types.h"
//#include "../sack/query.hpp"
#include "libdnf/rpm/package_set.hpp"
#include "libdnf/utils/sqlite3/sqlite3.hpp"

#include "comps_group.hpp"
#include "transaction.hpp"
#include "transaction_item.hpp"

namespace libdnf::transaction {

struct Swdb {
public:
    explicit Swdb(libdnf::utils::SQLite3 & conn);
    explicit Swdb(const std::string &path);
    ~Swdb();

    // Database
    // FIXME load this from conf
    static constexpr const char *defaultPath = "/var/lib/dnf/history.sqlite";
    static constexpr const char *defaultDatabaseName = "history.sqlite";

    const std::string &getPath() { return conn->get_path(); }
    void resetDatabase();
    void closeDatabase();

    // Transaction in progress
    void initTransaction();
    int64_t beginTransaction(int64_t dtBegin,
                             std::string rpmdbVersionBegin,
                             std::string cmdline,
                             uint32_t userId);
    int64_t endTransaction(int64_t dtEnd, std::string rpmdbVersionEnd, TransactionState state);
    int64_t closeTransaction();
    // TODO:
    //std::vector< TransactionItemPtr > getItems() { return transactionInProgress->getItems(); }

    const std::vector<std::unique_ptr<CompsEnvironment>> & get_comps_environments() const noexcept { return transactionInProgress->get_comps_environments(); }
    //CompsEnvironment & new_comps_environment();

    const std::vector<std::unique_ptr<CompsGroup>> & get_comps_groups() const noexcept { return transactionInProgress->get_comps_groups(); }
    //CompsGroup & new_comps_group();

    const std::vector<std::unique_ptr<Package>> & get_packages() const noexcept { return transactionInProgress->get_packages(); }
    //Package & new_package();

    TransactionPtr getLastTransaction();
    std::vector< TransactionPtr >
    listTransactions(); // std::vector<long long> transactionIds);

    // TransactionItems
    TransactionItemPtr addItem(ItemPtr item,
                               const std::string &repoid,
                               TransactionItemAction action,
                               TransactionItemReason reason);
    // std::shared_ptr<TransactionItem> replacedBy);

    // TODO: remove; TransactionItem states are saved on transaction save
    void setItemDone(const std::string &nevra);

    // Item: constructors
    /*
    PackagePtr createPackage();
    CompsGroupItemPtr createCompsGroupItem();
    CompsEnvironmentItemPtr createCompsEnvironmentItem();
    */

    const std::string getRPMRepo(const std::string &nevra);
    //TransactionItemPtr getRPMTransactionItem(const std::string &nevra);
    std::vector< int64_t > searchTransactionsByRPM(const std::vector< std::string > &patterns);

    // Item: CompsGroup
    TransactionItemPtr getCompsGroupItem(const std::string &groupid);
    std::vector< TransactionItemPtr > getCompsGroupItemsByPattern(const std::string &pattern);
    std::vector< std::string > getPackageCompsGroups(const std::string &packageName);

    // Item: CompsEnvironment
    TransactionItemPtr getCompsEnvironmentItem(const std::string &envid);
    std::vector< TransactionItemPtr > getCompsEnvironmentItemsByPattern(const std::string &pattern);
    std::vector< std::string > getCompsGroupEnvironments(const std::string &groupId);

    // misc
    void setReleasever(std::string value);
    void add_console_output_line(int file_descriptor, const std::string & line);

    /**
    * @brief Remove packages from PackageSet that were installed as Dependency or WEAK_DEPENDENCY
    */
    void filterUserinstalled(libdnf::rpm::PackageSet & installed) const;

    libdnf::utils::SQLite3 & get_connection() const { return *conn; }

    Transaction * get_transaction_in_progress() { return transactionInProgress.get(); }
protected:
    //friend class Transformer;

    explicit Swdb(libdnf::utils::SQLite3 & conn, bool autoClose);
    libdnf::utils::SQLite3 * conn;
    bool autoClose;
    std::unique_ptr< Transaction > transactionInProgress = nullptr;
    std::map< std::string, TransactionItemPtr > itemsInProgress;

private:
};

} // namespace libdnf::transaction

#endif // LIBDNF_TRANSACTION_SWDB_HPP

#endif
