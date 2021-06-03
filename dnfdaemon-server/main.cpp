// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "dbus.hpp"
#include "session_manager.hpp"

#include <sdbus-c++/sdbus-c++.h>

#include <iostream>

int main() {
    std::unique_ptr<sdbus::IConnection> connection = nullptr;
    try {
        connection = sdbus::createSystemBusConnection(dnfdaemon::DBUS_NAME);
    } catch (const sdbus::Error & e) {
        //std::cerr << tfm::format("Fatal error: %s", e.what()) << std::endl;
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    auto session_manager = SessionManager(*connection, dnfdaemon::DBUS_OBJECT_PATH);
    connection->enterEventLoop();
}
