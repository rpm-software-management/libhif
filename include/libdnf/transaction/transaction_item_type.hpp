// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_TRANSACTION_TRANSACTION_ITEM_TYPE_HPP
#define LIBDNF_TRANSACTION_TRANSACTION_ITEM_TYPE_HPP


namespace libdnf::transaction {


enum class TransactionItemType : int {
    UNKNOWN = 0,  // default type, must be changed before save
    RPM = 1,
    GROUP = 2,
    ENVIRONMENT = 3
};


}  // namespace libdnf::transaction


#endif  // LIBDNF_TRANSACTION_TRANSACTION_ITEM_TYPE_HPP
