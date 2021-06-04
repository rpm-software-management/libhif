// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "libdnf/transaction/transaction_item_action.hpp"


namespace libdnf::transaction {


std::string TransactionItemAction_get_name(TransactionItemAction action) {
    switch (action) {
        case TransactionItemAction::INSTALL:
            return "Install";
        case TransactionItemAction::DOWNGRADE:
            return "Downgrade";
        case TransactionItemAction::DOWNGRADED:
            return "Downgraded";
        case TransactionItemAction::OBSOLETE:
            return "Obsolete";
        case TransactionItemAction::OBSOLETED:
            return "Obsoleted";
        case TransactionItemAction::UPGRADE:
            return "Upgrade";
        case TransactionItemAction::UPGRADED:
            return "Upgraded";
        case TransactionItemAction::REMOVE:
            return "Removed";
        case TransactionItemAction::REINSTALL:
            return "Reinstall";
        case TransactionItemAction::REINSTALLED:
            return "Reinstalled";
        case TransactionItemAction::REASON_CHANGE:
            return "Reason Change";
    }
    return "";
}


std::string TransactionItemAction_get_short(TransactionItemAction action) {
    // TODO(dmach): consider adding the direction, e.g. ">U" == "Upgrade", "<U" == "Upgraded"
    switch (action) {
        case TransactionItemAction::INSTALL:
            return "I";
        case TransactionItemAction::DOWNGRADE:
            return "D";
        case TransactionItemAction::DOWNGRADED:
            return "D";
        case TransactionItemAction::OBSOLETE:
            return "O";
        case TransactionItemAction::OBSOLETED:
            return "O";
        case TransactionItemAction::UPGRADE:
            return "U";
        case TransactionItemAction::UPGRADED:
            return "U";
        case TransactionItemAction::REMOVE:
            // "R" is for Reinstall, therefore use "E" for rEmove (or Erase)
            return "E";
        case TransactionItemAction::REINSTALL:
            return "R";
        case TransactionItemAction::REINSTALLED:
            return "R";
        case TransactionItemAction::REASON_CHANGE:
            // TODO(dmach): replace "?" with something better
            return "?";
    }
    return "";
}


bool TransactionItemAction_is_forward_action(TransactionItemAction action) {
    switch (action) {
        case TransactionItemAction::INSTALL:
        case TransactionItemAction::DOWNGRADE:
        case TransactionItemAction::OBSOLETE:
        case TransactionItemAction::UPGRADE:
        case TransactionItemAction::REINSTALL:
            return true;
        default:
            return false;
    }
    return false;
}


bool TransactionItemAction_is_backward_action(TransactionItemAction action) {
    switch (action) {
        case TransactionItemAction::REMOVE:
        case TransactionItemAction::DOWNGRADED:
        case TransactionItemAction::OBSOLETED:
        case TransactionItemAction::UPGRADED:
        case TransactionItemAction::REINSTALLED:
            return true;
        default:
            return false;
    }
    return false;
}


}  // namespace libdnf::transaction
