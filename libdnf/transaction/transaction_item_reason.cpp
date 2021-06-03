// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "libdnf/transaction/transaction_item_reason.hpp"


namespace libdnf::transaction {


std::string TransactionItemReason_to_string(TransactionItemReason reason) {
    switch (reason) {
        case TransactionItemReason::UNKNOWN:
            return "unknown";
        case TransactionItemReason::DEPENDENCY:
            return "dependency";
        case TransactionItemReason::USER:
            return "user";
        case TransactionItemReason::CLEAN:
            return "clean";
        case TransactionItemReason::WEAK_DEPENDENCY:
            return "weak-dependency";
        case TransactionItemReason::GROUP:
            return "group";
    }
    return "";
}


static TransactionItemReason order[] = {
    TransactionItemReason::UNKNOWN,
    TransactionItemReason::CLEAN,
    TransactionItemReason::WEAK_DEPENDENCY,
    TransactionItemReason::DEPENDENCY,
    TransactionItemReason::GROUP,
    TransactionItemReason::USER,
};


bool operator<(TransactionItemReason lhs, TransactionItemReason rhs) {
    if (lhs == rhs) {
        return false;
    }
    for (auto reason : order) {
        // iterate through 'order' and return according to which value matches
        if (lhs == reason) {
            return true;
        }
        if (rhs == reason) {
            return false;
        }
    }
    return false;
}


bool operator<=(TransactionItemReason lhs, TransactionItemReason rhs) {
    if (lhs == rhs) {
        return true;
    }
    return lhs < rhs;
}


bool operator>(TransactionItemReason lhs, TransactionItemReason rhs) {
    if (lhs == rhs) {
        return false;
    }
    return rhs < lhs;
}


bool operator>=(TransactionItemReason lhs, TransactionItemReason rhs) {
    if (lhs == rhs) {
        return true;
    }
    return lhs > rhs;
}


int TransactionItemReason_compare(TransactionItemReason lhs, TransactionItemReason rhs) {
    if (lhs < rhs) {
        return -1;
    }
    if (lhs > rhs) {
        return 1;
    } else {
        return 0;
    }
}


}  // namespace libdnf::transaction
