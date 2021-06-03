// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef MICRODNF_COMMANDS_COMMAND_HPP
#define MICRODNF_COMMANDS_COMMAND_HPP

namespace microdnf {

class Context;

class Command {
public:
    virtual void set_argument_parser(Context &) {}
    virtual void pre_configure(Context &) {}
    virtual void configure(Context &) {}
    virtual void run(Context &) {}
    virtual ~Command() = default;
};

}  // namespace microdnf

#endif
