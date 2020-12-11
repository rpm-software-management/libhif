/*
Copyright (C) 2020 Red Hat, Inc.

This file is part of microdnf: https://github.com/rpm-software-management/libdnf/

Microdnf is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Microdnf is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with microdnf.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef MICRODNF_COMMANDS_INSTALL_INSTALL_HPP
#define MICRODNF_COMMANDS_INSTALL_INSTALL_HPP

#include "../command.hpp"

#include <libdnf/conf/option_bool.hpp>

#include <memory>
#include <set>

namespace microdnf {

class CmdInstall : public Command {
public:
    void set_argument_parser(Context & ctx) override;
    void configure(Context & ctx) override;
    void run(Context & ctx) override;

private:
    std::set<std::string> specs;
    std::set<std::string> package_paths;
};

}  // namespace microdnf

#endif
