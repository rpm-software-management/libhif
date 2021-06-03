// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_TRANSACTION_TRANSACTION_ITEM_STATE_HPP
#define LIBDNF_TRANSACTION_TRANSACTION_ITEM_STATE_HPP


namespace libdnf::transaction {


enum class TransactionItemState : int {
    UNKNOWN = 0,  // default state, must be changed before save
    DONE = 1,
    ERROR = 2
};


}  // namespace libdnf::transaction


#endif  // LIBDNF_TRANSACTION_TRANSACTION_ITEM_STATE_HPP
