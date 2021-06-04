// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef DNFDAEMON_CLIENT_WRAPPER_DBUS_PACKAGE_WRAPPER_HPP
#define DNFDAEMON_CLIENT_WRAPPER_DBUS_PACKAGE_WRAPPER_HPP

#include <dnfdaemon-server/dbus.hpp>
#include <libdnf/transaction/transaction_item_action.hpp>

#include <vector>

using namespace libdnf::transaction;

namespace dnfdaemon::client {

class DbusPackageWrapper {
public:
    explicit DbusPackageWrapper(const dnfdaemon::KeyValueMap & rawdata) : rawdata(rawdata){};

    int get_id() { return rawdata.at("id"); }
    std::string get_name() const { return rawdata.at("name"); }
    std::string get_epoch() const { return rawdata.at("epoch"); }
    std::string get_version() const { return rawdata.at("version"); }
    std::string get_release() const { return rawdata.at("release"); }
    std::string get_arch() const { return rawdata.at("arch"); }
    std::string get_repo_id() const { return rawdata.at("repo"); }
    std::string get_nevra() const { return rawdata.at("nevra"); }
    std::string get_full_nevra() const { return rawdata.at("full_nevra"); }
    bool is_installed() const { return rawdata.at("is_installed"); }
    uint64_t get_install_size() const { return rawdata.at("install_size"); }
    uint64_t get_package_size() const { return rawdata.at("package_size"); }
    std::string get_sourcerpm() const { return rawdata.at("sourcerpm"); }
    std::string get_summary() const { return rawdata.at("summary"); }
    std::string get_url() const { return rawdata.at("url"); }
    std::string get_license() const { return rawdata.at("license"); }
    std::string get_description() const { return rawdata.at("description"); }

private:
    dnfdaemon::KeyValueMap rawdata;
};

}  // namespace dnfdaemon::client

#endif  // DNFDAEMON_CLIENT_WRAPPER_DBUS_PACKAGE_WRAPPER_HPP
