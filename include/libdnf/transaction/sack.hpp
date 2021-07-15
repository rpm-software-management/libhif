/*
Copyright (C) 2020 Red Hat, Inc.

This file is part of libdnf: https://github.com/rpm-software-management/libdnf/

Libdnf is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Libdnf is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with libdnf.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef LIBDNF_TRANSACTION_SACK_HPP
#define LIBDNF_TRANSACTION_SACK_HPP

#include "query.hpp"
#include "transaction.hpp"

#include "libdnf/base/base_weak.hpp"
#include "libdnf/common/sack/sack.hpp"
#include "libdnf/common/weak_ptr.hpp"
#include "libdnf/transaction/db/db.hpp"

#include <mutex>


namespace libdnf::transaction {

/// Weak pointer to Transaction. It doesn't own the object (ptr_owner = false).
/// Transactions are owned by TransactionSack.
using TransactionWeakPtr = libdnf::WeakPtr<Transaction, false>;


using TransactionSackWeakPtr = libdnf::WeakPtr<TransactionSack, false>;


/// TransactionSack holds Transaction objects.
/// Unlike in other sacks, the data is loaded on-demand.
class TransactionSack : public libdnf::sack::Sack<Transaction> {
public:
    explicit TransactionSack(libdnf::Base & base);
    ~TransactionSack();

    /// Create a new Transaction object, store it in the sack and return a weak pointer to it.
    /// The Transaction object is owned by the TransactionSack.
    TransactionWeakPtr new_transaction();

    using libdnf::sack::Sack<Transaction>::get_data;

    TransactionSackWeakPtr get_weak_ptr() { return TransactionSackWeakPtr(this, &sack_guard); }

    /// @return The `Base` object to which this object belongs.
    /// @since 5.0
    libdnf::BaseWeakPtr get_base() const;

private:
    friend class Transaction;
    friend class TransactionQuery;
    libdnf::Base & base;

    WeakPtrGuard<TransactionSack, false> sack_guard;

    // Lazy adding new items to the sack needs to be thread-safe to avoid adding duplicates.
    std::mutex mtx;
};

}  // namespace libdnf::transaction

#endif  // LIBDNF_TRANSACTION_SACK_HPP
