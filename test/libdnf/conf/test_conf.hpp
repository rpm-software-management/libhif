// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef TEST_LIBDNF_CONF_CONF_HPP
#define TEST_LIBDNF_CONF_CONF_HPP

#include "../testcase_fixture.hpp"

#include "libdnf/conf/config_main.hpp"
#include "libdnf/conf/vars.hpp"
#include "libdnf/logger/log_router.hpp"

#include <cppunit/extensions/HelperMacros.h>


class ConfTest : public TestCaseFixture {
    CPPUNIT_TEST_SUITE(ConfTest);
    CPPUNIT_TEST(test_config_main);
    CPPUNIT_TEST(test_config_repo);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() override;

    void test_config_main();
    void test_config_repo();

    libdnf::Vars vars;
    libdnf::LogRouter logger;
    libdnf::ConfigMain config;
};


#endif
