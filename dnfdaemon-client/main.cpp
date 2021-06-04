// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "commands/downgrade/downgrade.hpp"
#include "commands/install/install.hpp"
#include "commands/remove/remove.hpp"
#include "commands/repolist/repolist.hpp"
#include "commands/repoquery/repoquery.hpp"
#include "commands/upgrade/upgrade.hpp"
#include "context.hpp"

#include <dnfdaemon-server/dbus.hpp>
#include <fcntl.h>
#include <fmt/format.h>
#include <libdnf-cli/argument_parser.hpp>
#include <libdnf/base/base.hpp>
#include <libdnf/logger/memory_buffer_logger.hpp>
#include <libdnf/logger/stream_logger.hpp>
#include <string.h>

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

namespace dnfdaemon::client {

static bool parse_args(Context & ctx, int argc, char * argv[]) {
    auto dnfdaemon_client = ctx.arg_parser.add_new_command("dnfdaemon_client");
    dnfdaemon_client->set_short_description("Utility for packages maintaining");
    dnfdaemon_client->set_description("Dnfdaemon-client is a program for maintaining packages.");
    dnfdaemon_client->set_commands_help_header("List of commands:");
    dnfdaemon_client->set_named_args_help_header("Global arguments:");

    auto help = ctx.arg_parser.add_new_named_arg("help");
    help->set_long_name("help");
    help->set_short_name('h');
    help->set_short_description("Print help");
    help->set_parse_hook_func([dnfdaemon_client](
                                  [[maybe_unused]] libdnf::cli::ArgumentParser::NamedArg * arg,
                                  [[maybe_unused]] const char * option,
                                  [[maybe_unused]] const char * value) {
        dnfdaemon_client->help();
        return true;
    });
    dnfdaemon_client->register_named_arg(help);

    // set ctx.verbose = true
    auto verbose = ctx.arg_parser.add_new_named_arg("verbose");
    verbose->set_short_name('v');
    verbose->set_long_name("verbose");
    verbose->set_short_description("increase output verbosity");
    verbose->set_const_value("true");
    verbose->link_value(&ctx.verbose);
    dnfdaemon_client->register_named_arg(verbose);

    auto assume_yes = ctx.arg_parser.add_new_named_arg("assumeyes");
    assume_yes->set_long_name("assumeyes");
    assume_yes->set_short_name('y');
    assume_yes->set_short_description("automatically answer yes for all questions");
    assume_yes->set_const_value("true");
    assume_yes->link_value(&ctx.assume_yes);
    dnfdaemon_client->register_named_arg(assume_yes);

    auto assume_no = ctx.arg_parser.add_new_named_arg("assumeno");
    assume_no->set_long_name("assumeno");
    assume_no->set_short_description("automatically answer no for all questions");
    assume_no->set_const_value("true");
    assume_no->link_value(&ctx.assume_no);
    dnfdaemon_client->register_named_arg(assume_no);

    auto allow_erasing = ctx.arg_parser.add_new_named_arg("allow_erasing");
    allow_erasing->set_long_name("allowerasing");
    allow_erasing->set_short_description("installed package can be removed to resolve the transaction");
    allow_erasing->set_const_value("true");
    allow_erasing->link_value(&ctx.allow_erasing);
    dnfdaemon_client->register_named_arg(allow_erasing);

    auto setopt = ctx.arg_parser.add_new_named_arg("setopt");
    setopt->set_long_name("setopt");
    setopt->set_has_value(true);
    setopt->set_arg_value_help("KEY=VALUE");
    setopt->set_short_description("set arbitrary config and repo options");
    setopt->set_description(
        R"**(Override a configuration option from the configuration file. To override configuration options for repositories, use repoid.option for  the
              <option>.  Values  for configuration options like excludepkgs, includepkgs, installonlypkgs and tsflags are appended to the original value,
              they do not override it. However, specifying an empty value (e.g. --setopt=tsflags=) will clear the option.)**");

    // --setopt option support
    setopt->set_parse_hook_func([&ctx](
                                    [[maybe_unused]] libdnf::cli::ArgumentParser::NamedArg * arg,
                                    [[maybe_unused]] const char * option,
                                    const char * value) {
        auto val = strchr(value + 1, '=');
        if (!val) {
            throw std::runtime_error(std::string("setopt: Badly formated argument value") + value);
        }
        auto key = std::string(value, val);
        auto dot_pos = key.rfind('.');
        if (dot_pos != std::string::npos) {
            if (dot_pos == key.size() - 1) {
                throw std::runtime_error(
                    std::string("setopt: Badly formated argument value: Last key character cannot be '.': ") + value);
            }
        }
        // Store option to vector for later use
        ctx.setopts.emplace_back(key, val + 1);
        return true;
    });
    dnfdaemon_client->register_named_arg(setopt);


    ctx.arg_parser.set_root_command(dnfdaemon_client);

    for (auto & command : ctx.commands) {
        command->set_argument_parser(ctx);
    }

    try {
        ctx.arg_parser.parse(argc, argv);
    } catch (const std::exception & ex) {
        std::cout << ex.what() << std::endl;
    }
    return help->get_parse_count() > 0;
}

}  // namespace dnfdaemon::client

int main(int argc, char * argv[]) {
    auto connection = sdbus::createSystemBusConnection();
    connection->enterEventLoopAsync();

    dnfdaemon::client::Context context(*connection);

    // TODO(mblaha): logging

    //log_router.info("Dnfdaemon-client start");

    // Register commands
    context.commands.push_back(std::make_unique<dnfdaemon::client::CmdRepolist>("repoinfo"));
    context.commands.push_back(std::make_unique<dnfdaemon::client::CmdRepolist>("repolist"));
    context.commands.push_back(std::make_unique<dnfdaemon::client::CmdRepoquery>());
    context.commands.push_back(std::make_unique<dnfdaemon::client::CmdDowngrade>());
    context.commands.push_back(std::make_unique<dnfdaemon::client::CmdInstall>());
    context.commands.push_back(std::make_unique<dnfdaemon::client::CmdUpgrade>());
    context.commands.push_back(std::make_unique<dnfdaemon::client::CmdRemove>());

    // Parse command line arguments
    bool help_printed = dnfdaemon::client::parse_args(context, argc, argv);
    if (!context.selected_command) {
        if (help_printed) {
            return 0;
        } else {
            context.arg_parser.get_root_command()->help();
            return 1;
        }
    }

    // initialize server session using command line arguments
    context.init_session();

    // Preconfigure selected command
    context.selected_command->pre_configure(context);

    // Configure selected command
    context.selected_command->configure(context);

    // Run selected command
    try {
        context.selected_command->run(context);
    } catch (std::exception & ex) {
        std::cerr << fmt::format("Command returned error: {}", ex.what()) << std::endl;
    }

    //log_router.info("Dnfdaemon-client end");

    return 0;
}
