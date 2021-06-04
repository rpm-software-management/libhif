// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_TRANSACTION_QUERY_HPP
#define LIBDNF_TRANSACTION_QUERY_HPP


#include "transaction.hpp"

#include "libdnf/common/sack/query.hpp"
#include "libdnf/common/weak_ptr.hpp"

#include <mutex>
#include <vector>


namespace libdnf::transaction {


class TransactionSack;


/// Weak pointer to Transaction. TransactionWeakPtr does not own it (ptr_owner = false).
/// Transaction objects are owned by TransactionSack.
using TransactionWeakPtr = libdnf::WeakPtr<Transaction, false>;


class TransactionQuery : public libdnf::sack::Query<TransactionWeakPtr> {
public:
    using libdnf::sack::Query<TransactionWeakPtr>::Query;

    TransactionQuery(TransactionSack & sack);

    /// @replaces libdnf:transaction/Transaction.hpp:method:Transaction.dbSelect(int64_t transaction_id)
    TransactionQuery & filter_id(int64_t pattern, sack::QueryCmp cmp = libdnf::sack::QueryCmp::EQ);
    TransactionQuery & filter_id(const std::vector<int64_t> & pattern, sack::QueryCmp cmp = libdnf::sack::QueryCmp::EQ);

private:
    friend TransactionSack;
    TransactionSack * sack = nullptr;

    // Load Transaction objects during first filter call.
    // The following filter calls only modify the previously created set of Transactions.
    bool initialized = false;

    struct F {
        static int64_t id(const TransactionWeakPtr & obj) { return obj->get_id(); }
    };
};


}  // namespace libdnf::transaction


#endif  // LIBDNF_TRANSACTION_QUERY_HPP
