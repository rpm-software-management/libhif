// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "libdnf/transaction/transaction.hpp"

#include "libdnf/transaction/comps_environment.hpp"
#include "libdnf/transaction/comps_group.hpp"
#include "libdnf/transaction/db/comps_environment.hpp"
#include "libdnf/transaction/db/comps_group.hpp"
#include "libdnf/transaction/db/console_output.hpp"
#include "libdnf/transaction/db/rpm.hpp"
#include "libdnf/transaction/db/trans.hpp"
#include "libdnf/transaction/db/trans_item.hpp"
#include "libdnf/transaction/db/trans_with.hpp"
#include "libdnf/transaction/rpm_package.hpp"
#include "libdnf/transaction/sack.hpp"
#include "libdnf/transaction/transaction_item.hpp"
#include "libdnf/utils/bgettext/bgettext-lib.h"

#include <fmt/format.h>


namespace libdnf::transaction {


Transaction::Transaction(TransactionSack & sack, int64_t pk) : sack{sack} {
    auto conn = transaction_db_connect(sack.base);

    auto query = trans_select_new_query(*conn);
    trans_select(*query, pk, *this);

    runtime_packages = load_transaction_runtime_packages(*conn, *this);
    console_output = console_output_load(*conn, *this);

    comps_environments = get_transaction_comps_environments(*conn, *this);
    comps_groups = get_transaction_comps_groups(*conn, *this);
    packages = get_transaction_packages(*conn, *this);
}


Transaction::Transaction(TransactionSack & sack) : sack{sack} {}


bool Transaction::operator==(const Transaction & other) const {
    return get_id() == other.get_id() && get_dt_start() == other.get_dt_start();
}


bool Transaction::operator<(const Transaction & other) const {
    return get_id() > other.get_id() || get_dt_start() > other.get_dt_start();
}


bool Transaction::operator>(const Transaction & other) const {
    return get_id() < other.get_id() || get_dt_start() < other.get_dt_start();
}


void Transaction::start() {
    if (id != 0) {
        throw std::runtime_error(_("Transaction has already started!"));
    }

    auto conn = transaction_db_connect(sack.base);
    conn->exec("BEGIN");
    try {
        auto query = trans_insert_new_query(*conn);
        trans_insert(*query, *this);

        save_transaction_runtime_packages(*conn, *this);

        insert_transaction_comps_environments(*conn, *this);
        insert_transaction_comps_groups(*conn, *this);
        insert_transaction_packages(*conn, *this);
        conn->exec("COMMIT");
    } catch (...) {
        conn->exec("ROLLBACK");
        throw;
    }
}


void Transaction::finish(TransactionState state) {
    // TODO(dmach): save item states
    /*
    // save states to the database before checking for UNKNOWN state
    for (auto i : getItems()) {
        i->saveState();
    }

    for (auto i : getItems()) {
        if (i->get_state() == TransactionItemState::UNKNOWN) {
            throw std::runtime_error(
                fmt::format(_("TransactionItem state is not set: {}"), i->getItem()->toStr()));
        }
    }
    */

    auto conn = transaction_db_connect(sack.base);
    conn->exec("BEGIN");
    try {
        set_state(state);
        auto query = trans_update_new_query(*conn);
        trans_update(*query, *this);
        conn->exec("COMMIT");
    } catch (...) {
        conn->exec("ROLLBACK");
        throw;
    }
}


void Transaction::add_console_output_line(int file_descriptor, const std::string & line) {
    if (!get_id()) {
        throw std::runtime_error(_("Can't add console output to unsaved transaction"));
    }

    auto conn = transaction_db_connect(sack.base);

    // save the line to the database
    console_output_insert_line(*conn, *this, file_descriptor, line);

    // also store the line in the console_output vector
    console_output.emplace_back(file_descriptor, line);
}


CompsEnvironment & Transaction::new_comps_environment() {
    auto env = std::make_unique<CompsEnvironment>(*this);
    // TODO(dmach): following lines are not thread-safe
    comps_environments.push_back(std::move(env));
    return *comps_environments.back();
}


CompsGroup & Transaction::new_comps_group() {
    auto grp = std::make_unique<CompsGroup>(*this);
    // TODO(dmach): following lines are not thread-safe
    comps_groups.push_back(std::move(grp));
    return *comps_groups.back();
}


Package & Transaction::new_package() {
    auto pkg = std::make_unique<Package>(*this);
    // TODO(dmach): following lines are not thread-safe
    packages.push_back(std::move(pkg));
    return *packages.back();
}


}  // namespace libdnf::transaction
