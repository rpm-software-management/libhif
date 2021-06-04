// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef DNFDAEMON_CLIENT_COMMANDS_INSTALL_INSTALL_HPP
#define DNFDAEMON_CLIENT_COMMANDS_INSTALL_INSTALL_HPP

#include "../command.hpp"

#include <libdnf/conf/option.hpp>
#include <libdnf/conf/option_bool.hpp>

namespace dnfdaemon::client {

class CmdInstall : public TransactionCommand {
public:
    void set_argument_parser(Context & ctx) override;
    void run(Context & ctx) override;

private:
    libdnf::OptionBool strict_option{false};
    std::vector<std::unique_ptr<libdnf::Option>> * patterns_options{nullptr};
};

}  // namespace dnfdaemon::client

#endif
