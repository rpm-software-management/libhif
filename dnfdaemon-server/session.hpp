// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef DNFDAEMON_SERVER_SESSION_HPP
#define DNFDAEMON_SERVER_SESSION_HPP

#include "dbus.hpp"
#include "threads_manager.hpp"
#include "utils.hpp"

#include <fmt/format.h>
#include <libdnf/base/base.hpp>
#include <libdnf/base/goal.hpp>
#include <sdbus-c++/sdbus-c++.h>

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class Session;

class IDbusSessionService {
public:
    explicit IDbusSessionService(Session & session) : session(session){};
    virtual ~IDbusSessionService() = default;
    virtual void dbus_register() = 0;

protected:
    Session & session;
};

class Session {
public:
    Session(
        sdbus::IConnection & connection,
        dnfdaemon::KeyValueMap session_configuration,
        std::string object_path,
        std::string sender);
    ~Session();

    template <typename ItemType>
    ItemType session_configuration_value(const std::string & key, const ItemType & default_value) {
        return key_value_map_get(session_configuration, key, default_value);
    }
    template <typename ItemType>
    ItemType session_configuration_value(const std::string & key) {
        return key_value_map_get<ItemType>(session_configuration, key);
    }

    std::string get_object_path() { return object_path; };
    sdbus::IConnection & get_connection() { return connection; };
    libdnf::Base * get_base() { return base.get(); };
    ThreadsManager & get_threads_manager() { return threads_manager; };
    sdbus::IObject * get_dbus_object() { return dbus_object.get(); };
    libdnf::Goal & get_goal() { return goal; };
    std::string get_sender() const { return sender; };

    bool check_authorization(const std::string & actionid, const std::string & sender);
    void fill_sack();
    bool read_all_repos();

    template <class S>
    void run_in_thread(S & service, sdbus::MethodReply (S::*method)(sdbus::MethodCall &&), sdbus::MethodCall && call) {
        auto worker = std::thread(
            [&method, &service, this](sdbus::MethodCall call) {
                try {
                    auto reply = (service.*method)(std::move(call));
                    reply.send();
                } catch (std::exception & ex) {
                    auto reply = call.createErrorReply({dnfdaemon::ERROR, ex.what()});
                    try {
                        reply.send();
                    } catch (const std::exception & e) {
                        auto & logger = *base->get_logger();
                        logger.error(fmt::format("Error sending d-bus error reply: {}", e.what()));
                    }
                }
                threads_manager.current_thread_finished();
            },
            std::move(call));
        threads_manager.register_thread(std::move(worker));
    }

private:
    sdbus::IConnection & connection;
    std::unique_ptr<libdnf::Base> base;
    libdnf::Goal goal;
    dnfdaemon::KeyValueMap session_configuration;
    std::string object_path;
    std::vector<std::unique_ptr<IDbusSessionService>> services{};
    ThreadsManager threads_manager;
    std::atomic<dnfdaemon::RepoStatus> repositories_status{dnfdaemon::RepoStatus::NOT_READY};
    std::unique_ptr<sdbus::IObject> dbus_object;
    std::string sender;
};

#endif
