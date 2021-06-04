// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "remove.hpp"

#include "../../context.hpp"
#include "../../utils.hpp"

#include <dnfdaemon-server/dbus.hpp>
#include <libdnf-cli/argument_parser.hpp>
#include <libdnf/conf/option_bool.hpp>
#include <libdnf/conf/option_string.hpp>

#include <iostream>
#include <memory>

namespace dnfdaemon::client {

void CmdRemove::set_argument_parser(Context & ctx) {
    auto remove = ctx.arg_parser.add_new_command("remove");
    remove->set_short_description("remove packages on the system");
    remove->set_description("");
    remove->set_named_args_help_header("Optional arguments:");
    remove->set_positional_args_help_header("Positional arguments:");
    remove->set_parse_hook_func([this, &ctx](
                                     [[maybe_unused]] libdnf::cli::ArgumentParser::Argument * arg,
                                     [[maybe_unused]] const char * option,
                                     [[maybe_unused]] int argc,
                                     [[maybe_unused]] const char * const argv[]) {
        ctx.select_command(this);
        return true;
    });
    ctx.arg_parser.get_root_command()->register_command(remove);

    patterns_options = ctx.arg_parser.add_new_values();
    auto keys = ctx.arg_parser.add_new_positional_arg(
        "keys_to_match",
        libdnf::cli::ArgumentParser::PositionalArg::UNLIMITED,
        ctx.arg_parser.add_init_value(std::unique_ptr<libdnf::Option>(new libdnf::OptionString(nullptr))),
        patterns_options);
    keys->set_short_description("List of packages to remove");
    remove->register_positional_arg(keys);

    // run remove command allways with allow_erasing on
    ctx.allow_erasing.set(libdnf::Option::Priority::RUNTIME, true);

}

void CmdRemove::run(Context & ctx) {
    if (!am_i_root()) {
        std::cout << "This command has to be run with superuser privileges (under the root user on most systems)."
                  << std::endl;
        return;
    }

    // get package specs from command line and add them to the goal
    std::vector<std::string> patterns;
    if (patterns_options->size() > 0) {
        patterns.reserve(patterns_options->size());
        for (auto & pattern : *patterns_options) {
            auto option = dynamic_cast<libdnf::OptionString *>(pattern.get());
            patterns.emplace_back(option->get_value());
        }
    }

    dnfdaemon::KeyValueMap options = {};

    ctx.session_proxy->callMethod("remove")
        .onInterface(dnfdaemon::INTERFACE_RPM)
        .withTimeout(static_cast<uint64_t>(-1))
        .withArguments(patterns, options);

    run_transaction(ctx);
}

}  // namespace dnfdaemon::client
