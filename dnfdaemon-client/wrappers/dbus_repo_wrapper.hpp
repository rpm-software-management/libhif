// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef DNFDAEMON_CLIENT_WRAPPERS_DBUS_REPO_WRAPPER_HPP
#define DNFDAEMON_CLIENT_WRAPPERS_DBUS_REPO_WRAPPER_HPP

#include <dnfdaemon-server/dbus.hpp>

#include <vector>

namespace dnfdaemon::client {

class DbusRepoWrapper {
public:
    explicit DbusRepoWrapper(dnfdaemon::KeyValueMap & rawdata) : rawdata(rawdata){};

    std::string get_id() const { return rawdata.at("id"); }
    std::string get_name() const { return rawdata.at("name"); }
    bool is_enabled() const { return rawdata.at("enabled"); }
    uint64_t get_size() const { return rawdata.at("size"); }
    std::string get_revision() const { return rawdata.at("revision"); }
    std::vector<std::pair<std::string, std::string>> get_distro_tags() const;
    int get_max_timestamp() const { return rawdata.at("updated"); }
    uint64_t get_pkgs() const { return rawdata.at("pkgs"); }
    uint64_t get_available_pkgs() const { return rawdata.at("available_pkgs"); }
    std::string get_metalink() const { return rawdata.at("metalink"); }
    std::string get_mirrorlist() const { return rawdata.at("mirrorlist"); }
    std::vector<std::string> get_baseurl() const { return rawdata.at("baseurl"); }
    int get_metadata_expire() const { return rawdata.at("metadata_expire"); }
    std::vector<std::string> get_excludepkgs() const { return rawdata.at("excludepkgs"); }
    std::vector<std::string> get_includepkgs() const { return rawdata.at("includepkgs"); }
    std::string get_repofile() const { return rawdata.at("repofile"); }

    std::vector<std::string> get_content_tags() const { return rawdata.at("content_tags"); }

private:
    dnfdaemon::KeyValueMap rawdata;
};

}  // namespace dnfdaemon::client

#endif  // DNFDAEMON_CLIENT_WRAPPERS_DBUS_REPO_WRAPPER_HPP
