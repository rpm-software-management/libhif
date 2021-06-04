// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef DNFDAEMON_CLIENT_CONTEXT_HPP
#define DNFDAEMON_CLIENT_CONTEXT_HPP

#include "callbacks.hpp"
#include "commands/command.hpp"

#include <dnfdaemon-server/dbus.hpp>
#include <libdnf-cli/argument_parser.hpp>
#include <libdnf/base/base.hpp>
#include <libdnf/conf/config.hpp>
#include <libdnf/repo/repo.hpp>
#include <sdbus-c++/sdbus-c++.h>

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

// TODO(mblaha) remove after microdnf is merged
namespace libdnf::repo {
using RepoWeakPtr = WeakPtr<Repo, false>;
using RepoSet = Set<RepoWeakPtr>;
}  // namespace libdnf::repo

namespace dnfdaemon::client {

constexpr const char * VERSION = "0.1.0";

class Context {
public:
    Context(sdbus::IConnection & connection)
        : connection(connection)
        , repositories_status(dnfdaemon::RepoStatus::NOT_READY){};

    /// Initialize dbus connection and server session
    void init_session();

    // initialize repository metadata loading on server side and wait for results
    dnfdaemon::RepoStatus wait_for_repos();

    // signal handlers
    void on_repositories_ready(const bool & result);

    /// Select command to execute
    void select_command(Command * cmd) { selected_command = cmd; }

    std::vector<std::pair<std::string, std::string>> setopts;
    std::vector<std::unique_ptr<Command>> commands;
    Command * selected_command{nullptr};
    libdnf::cli::ArgumentParser arg_parser;
    /// proxy to dnfdaemon session
    std::unique_ptr<sdbus::IProxy> session_proxy;

    // global command line arguments
    libdnf::OptionBool verbose{false};
    libdnf::OptionBool assume_yes{false};
    libdnf::OptionBool assume_no{false};
    libdnf::OptionBool allow_erasing{false};

private:
    /// system d-bus connection
    //std::unique_ptr<sdbus::IConnection> connection;
    sdbus::IConnection & connection;
    sdbus::ObjectPath session_object_path;
    dnfdaemon::RepoStatus repositories_status;
    std::unique_ptr<RepoCB> repocb;
    std::unique_ptr<PackageDownloadCB> package_download_cb;
    std::unique_ptr<TransactionCB> transaction_cb;
};

/// Asks the user for confirmation. The default answer is taken from the commandline options
bool userconfirm(Context & ctx);

}  // namespace dnfdaemon::client

#endif
