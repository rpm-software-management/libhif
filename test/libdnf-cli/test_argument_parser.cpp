// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "test_argument_parser.hpp"

#include "../libdnf/utils.hpp"

#include "libdnf-cli/argument_parser.hpp"

#include "libdnf/conf/option_bool.hpp"
#include "libdnf/conf/option_string.hpp"


CPPUNIT_TEST_SUITE_REGISTRATION(ArgumentParserTest);

using ArgParser = libdnf::cli::ArgumentParser;

void ArgumentParserTest::test_argument_parser() {
    ArgParser arg_parser;
    ArgParser::Command * selected_cmd{nullptr};

    auto * test = arg_parser.add_new_command("test");
    test->set_short_description("Unit test for testing ArgumentParser");
    test->set_description("Tets is a unit test for testing ArgumentParser.");
    test->set_commands_help_header("List of commands:");
    test->set_named_args_help_header("Global arguments:");

    arg_parser.set_root_command(test);

    auto * help = arg_parser.add_new_named_arg("help");
    help->set_long_name("help");
    help->set_short_name('h');
    help->set_short_description("Print help");
    help->set_parse_hook_func([test](
                                  [[maybe_unused]] ArgParser::NamedArg * arg,
                                  [[maybe_unused]] const char * option,
                                  [[maybe_unused]] const char * value) {
        test->help();
        return true;
    });
    test->register_named_arg(help);

    auto * global_arg = arg_parser.add_new_named_arg("global_arg");
    global_arg->set_long_name("global_arg");
    global_arg->set_short_description("Global argument for test");
    test->register_named_arg(global_arg);

    auto * available_option =
        dynamic_cast<libdnf::OptionBool *>(arg_parser.add_init_value(std::make_unique<libdnf::OptionBool>(true)));
    CPPUNIT_ASSERT(available_option);

    auto * installed_option =
        dynamic_cast<libdnf::OptionBool *>(arg_parser.add_init_value(std::make_unique<libdnf::OptionBool>(false)));
    CPPUNIT_ASSERT(installed_option);

    auto * info_option =
        dynamic_cast<libdnf::OptionBool *>(arg_parser.add_init_value(std::make_unique<libdnf::OptionBool>(false)));
    CPPUNIT_ASSERT(info_option);

    auto * nevra_option =
        dynamic_cast<libdnf::OptionBool *>(arg_parser.add_init_value(std::make_unique<libdnf::OptionBool>(true)));
    CPPUNIT_ASSERT(nevra_option);

    auto * available = arg_parser.add_new_named_arg("available");
    available->set_long_name("available");
    available->set_short_description("display available packages (default)");
    available->set_const_value("true");
    available->link_value(available_option);

    auto * installed = arg_parser.add_new_named_arg("installed");
    installed->set_long_name("installed");
    installed->set_short_description("display installed packages");
    installed->set_const_value("true");
    installed->link_value(installed_option);

    auto * info = arg_parser.add_new_named_arg("info");
    info->set_long_name("info");
    info->set_short_description("show detailed information about the packages");
    info->set_const_value("true");
    info->link_value(info_option);

    auto * nevra = arg_parser.add_new_named_arg("nevra");
    nevra->set_long_name("nevra");
    nevra->set_short_description(
        "use name-epoch:version-release.architecture format for displaying packages (default)");
    nevra->set_const_value("true");
    nevra->link_value(nevra_option);

    auto * keys_options = arg_parser.add_new_values();
    auto * keys = arg_parser.add_new_positional_arg(
        "keys",
        ArgParser::PositionalArg::UNLIMITED,
        arg_parser.add_init_value(std::make_unique<libdnf::OptionString>(nullptr)),
        keys_options);
    keys->set_short_description("List of keys to match");

    auto * conflict_args = arg_parser.add_conflict_args_group(
        std::unique_ptr<std::vector<ArgParser::Argument *>>(new std::vector<ArgParser::Argument *>{info, nevra}));

    info->set_conflict_arguments(conflict_args);
    nevra->set_conflict_arguments(conflict_args);

    auto * repoquery = arg_parser.add_new_command("repoquery");
    repoquery->set_short_description("search for packages matching keyword");
    repoquery->set_description("");
    repoquery->set_named_args_help_header("Optional arguments:");
    repoquery->set_positional_args_help_header("Positional arguments:");
    repoquery->set_parse_hook_func([&selected_cmd](
                                       ArgParser::Argument * arg,
                                       [[maybe_unused]] const char * option,
                                       [[maybe_unused]] int argc,
                                       [[maybe_unused]] const char * const argv[]) {
        selected_cmd = dynamic_cast<ArgParser::Command *>(arg);
        return true;
    });

    repoquery->register_named_arg(available);
    repoquery->register_named_arg(installed);
    repoquery->register_named_arg(info);
    repoquery->register_named_arg(nevra);
    repoquery->register_positional_arg(keys);

    test->register_command(repoquery);

    CPPUNIT_ASSERT_THROW(arg_parser.add_new_command("bad.id"), ArgParser::Argument::InvalidId);
    CPPUNIT_ASSERT_THROW(arg_parser.add_new_named_arg("bad.id"), ArgParser::Argument::InvalidId);
    CPPUNIT_ASSERT_THROW(
        arg_parser.add_new_positional_arg("bad.id", ArgParser::PositionalArg::UNLIMITED, nullptr, nullptr),
        ArgParser::Argument::InvalidId);

    CPPUNIT_ASSERT_EQUAL(test, arg_parser.get_root_command());

    CPPUNIT_ASSERT_EQUAL(repoquery, &arg_parser.get_command("repoquery"));
    CPPUNIT_ASSERT_THROW(arg_parser.get_command("unknowncmd"), ArgParser::Command::CommandNotFound);
    CPPUNIT_ASSERT_THROW(arg_parser.get_command("global_arg"), ArgParser::Command::CommandNotFound);

    CPPUNIT_ASSERT_EQUAL(global_arg, &arg_parser.get_named_arg("global_arg", false));
    CPPUNIT_ASSERT_EQUAL(global_arg, &arg_parser.get_named_arg("global_arg", true));
    CPPUNIT_ASSERT_EQUAL(installed, &arg_parser.get_named_arg("repoquery.installed", false));
    CPPUNIT_ASSERT_EQUAL(installed, &arg_parser.get_named_arg("repoquery.installed", true));
    CPPUNIT_ASSERT_THROW(arg_parser.get_named_arg("repoquery.global_arg", false), ArgParser::Command::NamedArgNotFound);
    CPPUNIT_ASSERT_EQUAL(global_arg, &arg_parser.get_named_arg("repoquery.global_arg", true));
    CPPUNIT_ASSERT_THROW(arg_parser.get_named_arg("unknowncmd.installed", false), ArgParser::Command::CommandNotFound);
    CPPUNIT_ASSERT_THROW(arg_parser.get_named_arg("unknowncmd.installed", true), ArgParser::Command::CommandNotFound);
    CPPUNIT_ASSERT_THROW(arg_parser.get_named_arg("repoquery.unknown", false), ArgParser::Command::NamedArgNotFound);
    CPPUNIT_ASSERT_THROW(arg_parser.get_named_arg("repoquery.unknown", true), ArgParser::Command::NamedArgNotFound);
    CPPUNIT_ASSERT_THROW(arg_parser.get_named_arg("repoquery.keys", false), ArgParser::Command::NamedArgNotFound);
    CPPUNIT_ASSERT_THROW(arg_parser.get_named_arg("repoquery.keys", true), ArgParser::Command::NamedArgNotFound);

    CPPUNIT_ASSERT_EQUAL(keys, &arg_parser.get_positional_arg("repoquery.keys", false));
    CPPUNIT_ASSERT_EQUAL(keys, &arg_parser.get_positional_arg("repoquery.keys", true));
    CPPUNIT_ASSERT_THROW(arg_parser.get_positional_arg("unknowncmd.keys", false), ArgParser::Command::CommandNotFound);
    CPPUNIT_ASSERT_THROW(arg_parser.get_positional_arg("unknowncmd.keys", true), ArgParser::Command::CommandNotFound);
    CPPUNIT_ASSERT_THROW(
        arg_parser.get_positional_arg("repoquery.unknown", false), ArgParser::Command::PositionalArgNotFound);
    CPPUNIT_ASSERT_THROW(
        arg_parser.get_positional_arg("repoquery.unknown", true), ArgParser::Command::PositionalArgNotFound);
    CPPUNIT_ASSERT_THROW(
        arg_parser.get_positional_arg("repoquery.installed", false), ArgParser::Command::PositionalArgNotFound);
    CPPUNIT_ASSERT_THROW(
        arg_parser.get_positional_arg("repoquery.installed", true), ArgParser::Command::PositionalArgNotFound);

    CPPUNIT_ASSERT_EQUAL((std::vector<ArgParser::Command *>{repoquery}), test->get_commands());
    CPPUNIT_ASSERT_EQUAL((std::vector<ArgParser::NamedArg *>{help, global_arg}), test->get_named_args());
    CPPUNIT_ASSERT_EQUAL((std::vector<ArgParser::PositionalArg *>{}), test->get_positional_args());
    CPPUNIT_ASSERT_EQUAL(repoquery, &test->get_command("repoquery"));
    CPPUNIT_ASSERT_THROW(test->get_command("unknowncmd"), ArgParser::Command::CommandNotFound);
    CPPUNIT_ASSERT_EQUAL(global_arg, &test->get_named_arg("global_arg"));
    CPPUNIT_ASSERT_THROW(test->get_named_arg("unknown"), ArgParser::Command::NamedArgNotFound);
    CPPUNIT_ASSERT_THROW(test->get_positional_arg("keys"), ArgParser::Command::PositionalArgNotFound);

    CPPUNIT_ASSERT_EQUAL((std::vector<ArgParser::Command *>{}), repoquery->get_commands());
    CPPUNIT_ASSERT_EQUAL(
        (std::vector<ArgParser::NamedArg *>{available, installed, info, nevra}), repoquery->get_named_args());
    CPPUNIT_ASSERT_EQUAL((std::vector<ArgParser::PositionalArg *>{keys}), repoquery->get_positional_args());
    CPPUNIT_ASSERT_THROW(repoquery->get_command("repoquery"), ArgParser::Command::CommandNotFound);
    CPPUNIT_ASSERT_EQUAL(info, &repoquery->get_named_arg("info"));
    CPPUNIT_ASSERT_THROW(repoquery->get_named_arg("unknown"), ArgParser::Command::NamedArgNotFound);
    CPPUNIT_ASSERT_EQUAL(keys, &repoquery->get_positional_arg("keys"));
    CPPUNIT_ASSERT_THROW(repoquery->get_positional_arg("unknown"), ArgParser::Command::PositionalArgNotFound);

    CPPUNIT_ASSERT_EQUAL(std::string("info"), info->get_id());
    CPPUNIT_ASSERT_EQUAL(info_option, dynamic_cast<libdnf::OptionBool *>(info->get_linked_value()));
    CPPUNIT_ASSERT_EQUAL(0, info->get_parse_count());

    CPPUNIT_ASSERT_EQUAL(std::string("keys"), keys->get_id());
    CPPUNIT_ASSERT_EQUAL(keys_options, keys->get_linked_values());
    CPPUNIT_ASSERT_EQUAL(0, info->get_parse_count());

    {
        constexpr const char * argv[]{"test", "repoquery", "--installed", "--info"};
        arg_parser.parse(std::size(argv), argv);

        CPPUNIT_ASSERT_EQUAL(repoquery, selected_cmd);

        auto * installed_linked_option = dynamic_cast<libdnf::OptionBool *>(installed->get_linked_value());
        CPPUNIT_ASSERT(installed_linked_option);
        CPPUNIT_ASSERT_EQUAL(true, installed_linked_option->get_value());
    }

    {
        // "--info" and "--nevra" cannot be used together
        constexpr const char * argv[]{"test", "repoquery", "--nevra", "--info"};
        CPPUNIT_ASSERT_THROW(arg_parser.parse(std::size(argv), argv), ArgParser::Conflict);
    }
}
