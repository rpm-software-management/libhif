// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "libdnf/transaction/sack.hpp"

#include "libdnf/base/base.hpp"


namespace libdnf::transaction {


TransactionSack::TransactionSack(libdnf::Base & base) : base{base} {}


TransactionSack::~TransactionSack() = default;


TransactionQuery TransactionSack::new_query() {
    // create an *empty* query
    // the content is lazily loaded/cached while running the queries
    TransactionQuery q;
    q.sack = this;
    return q;
}


TransactionWeakPtr TransactionSack::new_transaction() {
    return add_item_with_return(std::make_unique<Transaction>(*this));
}


}  // namespace libdnf::transaction
