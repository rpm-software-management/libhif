/*
 * Copyright (C) 2017-2019 Red Hat, Inc.
 *
 * Licensed under the GNU Lesser General Public License Version 2.1
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */


#include "TransactionItemReason.hpp"

#include <map>
#include <stdexcept>


namespace libdnf {

static const std::map<TransactionItemReason, std::string> transactionItemReasonName = {
    {TransactionItemReason::UNKNOWN, "unknown"},
    {TransactionItemReason::DEPENDENCY, "dependency"},
    {TransactionItemReason::USER, "user"},
    {TransactionItemReason::CLEAN, "clean"},
    {TransactionItemReason::WEAK_DEPENDENCY, "weak-dependency"},
    {TransactionItemReason::GROUP, "group"},
};

const std::string &
TransactionItemReasonToString(TransactionItemReason reason)
{
    try {
        return transactionItemReasonName.at(reason);
    } catch (std::out_of_range & e) {
        throw std::out_of_range("Transaction Item Reason ID \"" +
            std::to_string(static_cast<int>(reason)) + "\" not found.");
    }
}

TransactionItemReason StringToTransactionItemReason(const std::string & str)
{
    for (auto it = transactionItemReasonName.begin(); it != transactionItemReasonName.end(); ++it) {
        if (it->second == str) {
            return it->first;
        }
    }

    throw std::out_of_range("Transaction Item Reason \"" + str + "\" not found.");
}

} // namespace libdnf
