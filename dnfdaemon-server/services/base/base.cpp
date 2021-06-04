// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "base.hpp"

#include "dnfdaemon-server/dbus.hpp"
#include "dnfdaemon-server/utils.hpp"

#include <fmt/format.h>
#include <libdnf/repo/repo.hpp>
#include <sdbus-c++/sdbus-c++.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <thread>

void Base::dbus_register() {
    auto dbus_object = session.get_dbus_object();
    dbus_object->registerMethod(
        dnfdaemon::INTERFACE_BASE, "read_all_repos", "", "b", [this](sdbus::MethodCall call) -> void {
            session.run_in_thread(*this, &Base::read_all_repos, std::move(call));
        });
    dbus_object->registerSignal(dnfdaemon::INTERFACE_BASE, dnfdaemon::SIGNAL_REPO_LOAD_START, "ss");
    dbus_object->registerSignal(dnfdaemon::INTERFACE_BASE, dnfdaemon::SIGNAL_REPO_LOAD_PROGRESS, "suu");
    dbus_object->registerSignal(dnfdaemon::INTERFACE_BASE, dnfdaemon::SIGNAL_REPO_LOAD_END, "s");
}

sdbus::MethodReply Base::read_all_repos(sdbus::MethodCall && call) {
    bool retval = session.read_all_repos();
    auto reply = call.createReply();
    reply << retval;
    return reply;
}
