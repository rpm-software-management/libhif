// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_TEST_UTILS_SPAN_HPP
#define LIBDNF_TEST_UTILS_SPAN_HPP

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

class UtilsSpanTest : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE(UtilsSpanTest);
    CPPUNIT_TEST(test_span);
    CPPUNIT_TEST(test_dynamic_span);
    CPPUNIT_TEST(test_span_iterator);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() override;
    void tearDown() override;

    void test_span();
    void test_dynamic_span();
    void test_span_iterator();

private:
};

#endif  // LIBDNF_TEST_UTILS_SPAN_HPP
