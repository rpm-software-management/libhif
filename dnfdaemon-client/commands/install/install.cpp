// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "install.hpp"

#include "../../context.hpp"
#include "../../utils.hpp"

#include <dnfdaemon-server/dbus.hpp>
#include <libdnf-cli/argument_parser.hpp>
#include <libdnf/conf/option_bool.hpp>
#include <libdnf/conf/option_string.hpp>

#include <iostream>
#include <memory>

#include "libdnf-cli/output/transaction_table.hpp"

namespace dnfdaemon::client {

void CmdInstall::set_argument_parser(Context & ctx) {
    auto install = ctx.arg_parser.add_new_command("install");
    install->set_short_description("install packages on the system");
    install->set_description("");
    install->set_named_args_help_header("Optional arguments:");
    install->set_positional_args_help_header("Positional arguments:");
    install->set_parse_hook_func([this, &ctx](
                                     [[maybe_unused]] libdnf::cli::ArgumentParser::Argument * arg,
                                     [[maybe_unused]] const char * option,
                                     [[maybe_unused]] int argc,
                                     [[maybe_unused]] const char * const argv[]) {
        ctx.select_command(this);
        return true;
    });
    ctx.arg_parser.get_root_command()->register_command(install);

    auto strict = ctx.arg_parser.add_new_named_arg("strict");
    strict->set_long_name("strict");
    strict->set_short_description(
        "Broken or unavailable packages cause the transaction to fail (yes) or will be skipped (no).");
    strict->set_has_value(true);
    strict->set_arg_value_help("<yes|no>");
    strict->link_value(&strict_option);
    install->register_named_arg(strict);

    patterns_options = ctx.arg_parser.add_new_values();
    auto keys = ctx.arg_parser.add_new_positional_arg(
        "keys_to_match",
        libdnf::cli::ArgumentParser::PositionalArg::UNLIMITED,
        ctx.arg_parser.add_init_value(std::unique_ptr<libdnf::Option>(new libdnf::OptionString(nullptr))),
        patterns_options);
    keys->set_short_description("List of packages to install");
    install->register_positional_arg(keys);
}

void CmdInstall::run(Context & ctx) {
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
    // pass the `strict` value to the server only when explicitly set by command line option
    if (strict_option.get_priority() >= libdnf::Option::Priority::COMMANDLINE) {
        options["strict"] = strict_option.get_value();
    }

    ctx.session_proxy->callMethod("install")
        .onInterface(dnfdaemon::INTERFACE_RPM)
        .withTimeout(static_cast<uint64_t>(-1))
        .withArguments(patterns, options);

    run_transaction(ctx);
}

}  // namespace dnfdaemon::client
