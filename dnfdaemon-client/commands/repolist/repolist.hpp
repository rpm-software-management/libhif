// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef DNFDAEMON_CLIENT_COMMANDS_REPOLIST_REPOLIST_HPP
#define DNFDAEMON_CLIENT_COMMANDS_REPOLIST_REPOLIST_HPP

#include "../command.hpp"

#include <libdnf/conf/option_enum.hpp>

#include <memory>
#include <vector>

namespace dnfdaemon::client {

class CmdRepolist : public Command {
public:
    CmdRepolist(const char * command) : command(command) {};
    void set_argument_parser(Context & ctx) override;
    void run(Context & ctx) override;

private:
    libdnf::OptionEnum<std::string> * enable_disable_option{nullptr};
    std::vector<std::unique_ptr<libdnf::Option>> * patterns_options{nullptr};
    const std::string command;
};

}  // namespace dnfdaemon::client

#endif
