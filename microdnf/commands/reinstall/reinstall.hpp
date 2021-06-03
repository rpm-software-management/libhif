// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef MICRODNF_COMMANDS_REINSTALL_REINSTALL_HPP
#define MICRODNF_COMMANDS_REINSTALL_REINSTALL_HPP

#include "../command.hpp"

#include <libdnf/conf/option_bool.hpp>

#include <memory>
#include <vector>

namespace microdnf {

class CmdReinstall : public Command {
public:
    void set_argument_parser(Context & ctx) override;
    void configure(Context & ctx) override;
    void run(Context & ctx) override;

private:
    std::vector<std::unique_ptr<libdnf::Option>> * patterns_to_reinstall_options{nullptr};
};

}  // namespace microdnf

#endif
