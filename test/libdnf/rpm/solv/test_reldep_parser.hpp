// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_TEST_RPM_SOLV_RELDEP_PARSER_HPP
#define LIBDNF_TEST_RPM_SOLV_RELDEP_PARSER_HPP


#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>


class ReldepParserTest : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE(ReldepParserTest);
    CPPUNIT_TEST(test_parser);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() override;
    void tearDown() override;

    void test_parser();

private:
};

#endif // LIBDNF_TEST_RPM_SOLV_RELDEP_PARSER_HPP
