// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef DNFDAEMON_SERVER_SERVICES_REPOCONF_CONFIGURATION_HPP
#define DNFDAEMON_SERVER_SERVICES_REPOCONF_CONFIGURATION_HPP

#include "dnfdaemon-server/session.hpp"

#include "libdnf/conf/config_main.hpp"
#include "libdnf/conf/config_parser.hpp"
#include "libdnf/repo/config_repo.hpp"

#include <map>
#include <memory>
#include <vector>


class Configuration {
public:
    struct RepoInfo {
        std::string file_path;
        std::unique_ptr<libdnf::repo::ConfigRepo> repoconfig;
    };

    explicit Configuration(Session & session);
    ~Configuration() = default;

    void read_configuration();
    const std::map<std::string, std::unique_ptr<RepoInfo>> & get_repos() { return repos; }
    RepoInfo * find_repo(const std::string & repoid);
    libdnf::ConfigParser * find_parser(const std::string & file_path);

private:
    // repoid: repoinfo
    std::map<std::string, std::unique_ptr<RepoInfo>> repos;
    // repo_config_file_path: parser
    std::map<std::string, std::unique_ptr<libdnf::ConfigParser>> config_parsers;
    std::map<std::string, std::string> substitutions;
    Session & session;

    void read_repos(const libdnf::ConfigParser * repo_parser, const std::string & file_path);
    void read_main_config();
    void read_repo_configs();
};


#endif
