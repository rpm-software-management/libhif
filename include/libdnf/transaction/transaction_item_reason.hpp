// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_TRANSACTION_TRANSACTION_ITEM_REASON_HPP
#define LIBDNF_TRANSACTION_TRANSACTION_ITEM_REASON_HPP


#include <string>


namespace libdnf::transaction {


enum class TransactionItemReason : int {
    UNKNOWN = 0,
    DEPENDENCY = 1,
    USER = 2,
    CLEAN = 3,  // hawkey compatibility
    WEAK_DEPENDENCY = 4,
    GROUP = 5
};


std::string TransactionItemReason_to_string(TransactionItemReason reason);


/// Compare transaction items and return:
/// -1 if lhs < rhs
/// 1 if lhs > rhs
/// 0 if lhs == rhs
/// Higher number means a better (or a stronger) reason.
int TransactionItemReason_compare(TransactionItemReason lhs, TransactionItemReason rhs);


bool operator<(TransactionItemReason lhs, TransactionItemReason rhs);
bool operator<=(TransactionItemReason lhs, TransactionItemReason rhs);
bool operator>(TransactionItemReason lhs, TransactionItemReason rhs);
bool operator>=(TransactionItemReason lhs, TransactionItemReason rhs);


}  // namespace libdnf::transaction


#endif  // LIBDNF_TRANSACTION_TRANSACTION_ITEM_REASON_HPP
