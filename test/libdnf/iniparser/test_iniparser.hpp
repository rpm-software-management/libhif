// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_TEST_INIPARSER_HPP
#define LIBDNF_TEST_INIPARSER_HPP


#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>


class IniparserTest : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE(IniparserTest);

    #ifndef WITH_PERFORMANCE_TESTS
    CPPUNIT_TEST(test_iniparser);
    CPPUNIT_TEST(test_iniparser2);
    #endif

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() override;
    void tearDown() override;

    void test_iniparser();
    void test_iniparser2();

private:
};

#endif
