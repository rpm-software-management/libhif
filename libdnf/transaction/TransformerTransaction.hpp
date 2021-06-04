// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_TRANSACTION_TRANSFORMERTRANSACTION_HPP
#define LIBDNF_TRANSACTION_TRANSFORMERTRANSACTION_HPP


#include "transaction.hpp"

#include "libdnf/transaction/db/trans_with.hpp"


namespace libdnf::transaction {

/**
 * Class overrides default behavior with
 * inserting rows with explicitly set IDs
 */
class TransformerTransaction : public Transaction {
public:
    using Transaction::Transaction;
    void start()
    {
        dbInsert();
        save_transaction_runtime_packages(*this);
        saveItems();
    }
};

}  // namespace libdnf::transaction

#endif // LIBDNF_TRANSACTION_TRANSFORMERTRANSACTION_HPP
