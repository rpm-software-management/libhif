// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_TRANSACTION_TRANSACTION_ITEM_HPP
#define LIBDNF_TRANSACTION_TRANSACTION_ITEM_HPP


#include "transaction_item_action.hpp"
#include "transaction_item_reason.hpp"
#include "transaction_item_state.hpp"
#include "transaction_item_type.hpp"

#include <string>


namespace libdnf::transaction {


class Transaction;


class TransactionItem {
public:
    using Action = TransactionItemAction;
    using Reason = TransactionItemReason;
    using State = TransactionItemState;
    using Type = TransactionItemType;

    explicit TransactionItem(Transaction & trans, Type item_type);

    /// Get database id (primary key) of the transaction item (table 'trans_item')
    int64_t get_id() const noexcept { return id; }

    /// Set database id (primary key) of the transaction item (table 'trans_item')
    void set_id(int64_t value) { id = value; }

    /// Get action associated with the transaction item in the transaction
    ///
    /// @replaces libdnf:transaction/TransactionItem.hpp:method:TransactionItemBase.getAction()
    Action get_action() const noexcept { return action; }

    /// Set action associated with the transaction item in the transaction
    ///
    /// @replaces libdnf:transaction/TransactionItem.hpp:method:TransactionItemBase.setAction(libdnf::TransactionItemAction value)
    void set_action(Action value) { action = value; }

    /// Get name of the action associated with the transaction item in the transaction
    ///
    /// @replaces libdnf:transaction/TransactionItem.hpp:method:TransactionItemBase.getActionName()
    std::string get_action_name();

    /// Get abbreviated name of the action associated with the transaction item in the transaction
    ///
    /// @replaces libdnf:transaction/TransactionItem.hpp:method:TransactionItemBase.getActionShort()
    std::string get_action_short();

    /// Get reason of the action associated with the transaction item in the transaction
    ///
    /// @replaces libdnf:transaction/TransactionItem.hpp:method:TransactionItemBase.getReason()
    Reason get_reason() const noexcept { return reason; }

    /// Set reason of the action associated with the transaction item in the transaction
    ///
    /// @replaces libdnf:transaction/TransactionItem.hpp:method:TransactionItemBase.setReason(libdnf::TransactionItemReason value)
    void set_reason(Reason value) { reason = value; }

    /// Get transaction item state
    ///
    /// @replaces libdnf:transaction/TransactionItem.hpp:method:TransactionItemBase.getState()
    State get_state() const noexcept { return state; }

    /// Set transaction item state
    ///
    /// @replaces libdnf:transaction/TransactionItem.hpp:method:TransactionItemBase.setState(libdnf::TransactionItemState value)
    void set_state(State value) { state = value; }

    /// Get transaction item repoid (text identifier of a repository)
    ///
    /// @replaces libdnf:transaction/TransactionItem.hpp:method:TransactionItemBase.getRepoid()
    const std::string & get_repoid() const noexcept { return repoid; }

    /// Get transaction item repoid (text identifier of a repository)
    ///
    /// @replaces libdnf:transaction/TransactionItem.hpp:method:TransactionItemBase.setRepoid(const std::string & value)
    void set_repoid(const std::string & value) { repoid = value; }

    /// Has the item appeared on the system during the transaction?
    ///
    /// @replaces libdnf:transaction/TransactionItem.hpp:method:TransactionItemBase.isForwardAction()
    bool is_forward_action() const;

    /// Has the item got removed from the system during the transaction?
    ///
    /// @replaces libdnf:transaction/TransactionItem.hpp:method:TransactionItemBase.isBackwardAction()
    bool is_backward_action() const;

    /// Get database id (primary key) of the item (table 'item'; other item tables such 'rpm' inherit from it via 1:1 relation)
    int64_t get_item_id() const noexcept { return item_id; }

    /// Set database id (primary key) of the item (table 'item'; other item tables such 'rpm' inherit from it via 1:1 relation)
    void set_item_id(int64_t value) { item_id = value; }

    /// Get an enum value representing the item type in the database table 'item'
    ///
    /// @replaces libdnf:transaction/Item.hpp:method:Item.getItemType()
    /// @replaces libdnf:transaction/CompsEnvironmentItem.hpp:method:CompsEnvironmentItem.getItemType()
    /// @replaces libdnf:transaction/CompsGroupItem.hpp:method:CompsGroupItem.getItemType()
    /// @replaces libdnf:transaction/RPMItem.hpp:method:RPMItem.getItemType()
    Type get_item_type() const noexcept { return item_type; }

    // TODO(dmach): move to sack, resolve for all packages; return the user who initially installed the package
    /// @replaces libdnf:transaction/TransactionItem.hpp:method:TransactionItem.getInstalledBy()
    uint32_t getInstalledBy() const;

    // TODO(dmach): Reimplement in Package class
    //const std::vector< TransactionItemPtr > &getReplacedBy() const noexcept { return replacedBy; }
    //void addReplacedBy(TransactionItemPtr value) { if (value) replacedBy.push_back(value); }
    //void saveReplacedBy();

    // TODO(dmach): Review and bring back if needed
    //void saveState();

    Transaction & get_transaction() { return trans; }

protected:
    int64_t id = 0;
    Action action = Action::INSTALL;
    Reason reason = Reason::UNKNOWN;
    State state = State::UNKNOWN;
    std::string repoid;

    int64_t item_id = 0;
    const Type item_type;

    Transaction & trans;

    // TODO(dmach): Reimplement in Package class; it's most likely not needed in Comps{Group,Environment}
    // std::vector< TransactionItemPtr > replacedBy;
};


}  // namespace libdnf::transaction


#endif  // LIBDNF_TRANSACTION_TRANSACTION_ITEM_HPP
