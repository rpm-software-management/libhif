// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef DNFDAEMON_SERVER_SESSIONMANAGER_HPP
#define DNFDAEMON_SERVER_SESSIONMANAGER_HPP

#include "session.hpp"
#include "threads_manager.hpp"

#include <sdbus-c++/sdbus-c++.h>

#include <map>
#include <memory>
#include <mutex>
#include <string>

class SessionManager {
public:
    SessionManager(sdbus::IConnection & connection, const std::string & object_path);
    ~SessionManager();

private:
    std::string object_path;
    sdbus::IConnection & connection;
    ThreadsManager threads_manager;
    std::unique_ptr<sdbus::IObject> dbus_object;
    std::unique_ptr<sdbus::IProxy> name_changed_proxy;

    std::mutex sessions_mutex;
    // map {sender_address: {session_id: Session object}}
    std::map<std::string, std::map<std::string, std::unique_ptr<Session>>> sessions;

    void dbus_register();
    void open_session(sdbus::MethodCall call);
    void close_session(sdbus::MethodCall call);
    void on_name_owner_changed(sdbus::Signal & signal);
};

#endif
