// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef DNFDAEMON_CLIENT_COMMANDS_REPOQUERY_REPOQUERY_HPP
#define DNFDAEMON_CLIENT_COMMANDS_REPOQUERY_REPOQUERY_HPP

#include "../command.hpp"

#include <libdnf/conf/option_bool.hpp>

#include <memory>
#include <vector>

namespace dnfdaemon::client {

class CmdRepoquery : public Command {
public:
    void set_argument_parser(Context & ctx) override;
    void run(Context & ctx) override;
    dnfdaemon::KeyValueMap session_config(Context &) override;

private:
    libdnf::OptionBool * available_option{nullptr};
    libdnf::OptionBool * installed_option{nullptr};
    libdnf::OptionBool * info_option{nullptr};
    std::vector<std::unique_ptr<libdnf::Option>> * patterns_options{nullptr};
};

}  // namespace dnfdaemon::client

#endif
