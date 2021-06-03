// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef TEST_LIBDNF_CONF_VARS_HPP
#define TEST_LIBDNF_CONF_VARS_HPP

#include "../testcase_fixture.hpp"

#include "libdnf/conf/vars.hpp"

#include <cppunit/extensions/HelperMacros.h>


class VarsTest : public TestCaseFixture {
    CPPUNIT_TEST_SUITE(VarsTest);
    CPPUNIT_TEST(test_vars);
    CPPUNIT_TEST(test_vars_multiple_dirs);
    CPPUNIT_TEST(test_vars_env);
    CPPUNIT_TEST_SUITE_END();

public:
    void test_vars();
    void test_vars_multiple_dirs();
    void test_vars_env();

    libdnf::Vars vars;
};


#endif
