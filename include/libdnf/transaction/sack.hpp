// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_TRANSACTION_SACK_HPP
#define LIBDNF_TRANSACTION_SACK_HPP


#include "query.hpp"
#include "transaction.hpp"

#include "libdnf/common/sack/sack.hpp"
#include "libdnf/transaction/db/db.hpp"

#include <mutex>


namespace libdnf {
class Base;
}


namespace libdnf::transaction {


/// Weak pointer to Transaction. It doesn't own the object (ptr_owner = false).
/// Transactions are owned by TransactionSack.
using TransactionWeakPtr = libdnf::WeakPtr<Transaction, false>;


using TransactionSackWeakPtr = libdnf::WeakPtr<TransactionSack, false>;


/// TransactionSack holds Transaction objects.
/// Unlike in other sacks, the data is loaded on-demand.
class TransactionSack : public libdnf::sack::Sack<Transaction, TransactionQuery> {
public:
    explicit TransactionSack(libdnf::Base & base);
    ~TransactionSack();

    TransactionQuery new_query();

    /// Create a new Transaction object, store it in the sack and return a weak pointer to it.
    /// The Transaction object is owned by the TransactionSack.
    TransactionWeakPtr new_transaction();

    using libdnf::sack::Sack<Transaction, TransactionQuery>::get_data;

    TransactionSackWeakPtr get_weak_ptr();

private:
    friend class Transaction;
    friend class TransactionQuery;
    libdnf::Base & base;

    WeakPtrGuard<TransactionSack, false> data_guard;

    // Lazy adding new items to the sack needs to be thread-safe to avoid adding duplicates.
    std::mutex mtx;
};

inline TransactionSackWeakPtr TransactionSack::get_weak_ptr() { return TransactionSackWeakPtr(this, &data_guard); }

}  // namespace libdnf::transaction


#endif  // LIBDNF_TRANSACTION_SACK_HPP
