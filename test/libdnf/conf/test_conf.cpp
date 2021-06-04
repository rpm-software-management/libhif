// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "test_conf.hpp"

#include "../utils.hpp"

#include "libdnf/repo/config_repo.hpp"


CPPUNIT_TEST_SUITE_REGISTRATION(ConfTest);

using namespace libdnf;

void ConfTest::setUp() {
    TestCaseFixture::setUp();
    ConfigParser parser;
    parser.read(PROJECT_SOURCE_DIR "/test/libdnf/conf/data/main.conf");
    config.load_from_parser(parser, "main", vars, logger);
}

void ConfTest::test_config_main() {
    CPPUNIT_ASSERT_EQUAL(7, config.debuglevel().get_value());
    CPPUNIT_ASSERT_EQUAL(std::string("hello"), config.persistdir().get_value());
    CPPUNIT_ASSERT_EQUAL(false, config.plugins().get_value());
    std::vector<std::string> pluginpath = {"/foo", "/bar"};
    CPPUNIT_ASSERT_EQUAL(pluginpath, config.pluginpath().get_value());
}

void ConfTest::test_config_repo() {
    repo::ConfigRepo config_repo(config);
    ConfigParser parser;
    parser.read(PROJECT_SOURCE_DIR "/test/libdnf/conf/data/main.conf");
    vars.load("/", {PROJECT_SOURCE_DIR "/test/libdnf/conf/data/vars"});
    config_repo.load_from_parser(parser, "repo-1", vars, logger);

    std::vector<std::string> baseurl = {"http://example.com/value123", "http://example.com/456"};
    CPPUNIT_ASSERT_EQUAL(baseurl, config_repo.baseurl().get_value());
}
