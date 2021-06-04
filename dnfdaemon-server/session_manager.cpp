// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "session_manager.hpp"

#include "dbus.hpp"
#include "session.hpp"

#include <sdbus-c++/sdbus-c++.h>

#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <thread>

SessionManager::SessionManager(sdbus::IConnection & connection, const std::string & object_path)
    : object_path(object_path)
    , connection(connection) {
    dbus_register();
}

SessionManager::~SessionManager() {
    dbus_object->unregister();
    threads_manager.finish();
}

void SessionManager::dbus_register() {
    dbus_object = sdbus::createObject(connection, object_path);
    dbus_object->registerMethod(
        dnfdaemon::INTERFACE_SESSION_MANAGER, "open_session", "a{sv}", "o", [this](sdbus::MethodCall call) -> void {
            this->open_session(std::move(call));
        });
    dbus_object->registerMethod(
        dnfdaemon::INTERFACE_SESSION_MANAGER, "close_session", "o", "b", [this](sdbus::MethodCall call) -> void {
            this->close_session(std::move(call));
        });
    dbus_object->finishRegistration();

    // register signal handler for NameOwnerChanged
    name_changed_proxy = sdbus::createProxy(connection, "org.freedesktop.DBus", "/org/freedesktop/DBus");
    name_changed_proxy->registerSignalHandler(
        "org.freedesktop.DBus", "NameOwnerChanged", [this](sdbus::Signal & signal) -> void {
            this->on_name_owner_changed(signal);
        });
    name_changed_proxy->finishRegistration();
}


std::string gen_session_id() {
    static std::random_device rd;
    static std::uniform_int_distribution<> dist(0, 15);

    std::stringstream ss;
    ss << std::hex;
    for (int i = 0; i < 32; i++) {
        ss << dist(rd);
    }
    return ss.str();
}


void SessionManager::on_name_owner_changed(sdbus::Signal & signal) {
    std::string name;
    std::string old_owner;
    std::string new_owner;
    signal >> name >> old_owner >> new_owner;
    if (new_owner.empty() && sessions.count(old_owner) > 0) {
        auto worker = std::thread([this, old_owner=std::move(old_owner)]() {
            std::map<std::string, std::map<std::string, std::unique_ptr<Session>>> to_be_erased;
            {
                std::lock_guard<std::mutex> lock(sessions_mutex);
                // the sender name disappeared from the dbus, erase all its sessions
                to_be_erased[old_owner] = std::move(sessions.at(old_owner));
                sessions.erase(old_owner);
            }
            to_be_erased.erase(old_owner);
            threads_manager.current_thread_finished();
        });
        threads_manager.register_thread(std::move(worker));
    }
}

void SessionManager::open_session(sdbus::MethodCall call) {
    auto sender = call.getSender();
    dnfdaemon::KeyValueMap configuration;
    call >> configuration;

    auto worker = std::thread([this,sender=std::move(sender),configuration=std::move(configuration),call=std::move(call)]() {
        // generate UUID-like session id
        const std::string sessionid = object_path + "/" + gen_session_id();
        // store newly created session
        {
            std::lock_guard<std::mutex> lock(sessions_mutex);
            sessions[std::move(sender)].emplace(
                sessionid, std::make_unique<Session>(connection, std::move(configuration), sessionid, sender));
        }

        auto reply = call.createReply();
        reply << sdbus::ObjectPath{sessionid};
        reply.send();
        threads_manager.current_thread_finished();
    });
    threads_manager.register_thread(std::move(worker));
}


void SessionManager::close_session(sdbus::MethodCall call) {
    auto sender = call.getSender();
    sdbus::ObjectPath session_id;
    call >> session_id;

    auto worker = std::thread([this,sender=std::move(sender),session_id=std::move(session_id),call=std::move(call)]() {
        bool retval = false;
        // find sessions created by the same sender
        auto sender_it = sessions.find(sender);
        if (sender_it != sessions.end()) {
            std::lock_guard<std::mutex> lock(sessions_mutex);
            // delete session with given session_id
            if (sender_it->second.erase(session_id) > 0) {
                retval = true;
            }
        }

        auto reply = call.createReply();
        reply << retval;
        reply.send();
        threads_manager.current_thread_finished();
    });
    threads_manager.register_thread(std::move(worker));
}
