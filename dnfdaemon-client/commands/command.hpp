// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef DNFDAEMON_CLIENT_COMMANDS_COMMAND_HPP
#define DNFDAEMON_CLIENT_COMMANDS_COMMAND_HPP

#include <dnfdaemon-server/dbus.hpp>

namespace dnfdaemon::client {

class Context;

class Command {
public:
    virtual void set_argument_parser(Context &) {}
    virtual dnfdaemon::KeyValueMap session_config(Context &) {
        dnfdaemon::KeyValueMap cfg = {};
        return cfg;
    }
    virtual void pre_configure(Context &) {}
    virtual void configure(Context &) {}
    virtual void run(Context &) {}
    virtual ~Command() = default;
};

class TransactionCommand : public Command {
public:
    void print_transaction(std::vector<dnfdaemon::DbusTransactionItem> transaction);
    void run_transaction(Context & ctx);
};


}  // namespace dnfdaemon::client

#endif
