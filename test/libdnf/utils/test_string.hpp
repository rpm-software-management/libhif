// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_TEST_UTILS_STRING_HPP
#define LIBDNF_TEST_UTILS_STRING_HPP


#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>


class UtilsStringTest : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE(UtilsStringTest);
    CPPUNIT_TEST(test_starts_with);
    CPPUNIT_TEST(test_ends_with);
    CPPUNIT_TEST(test_join);
    CPPUNIT_TEST(test_split);
    CPPUNIT_TEST(test_rsplit);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() override;
    void tearDown() override;

    void test_starts_with();
    void test_ends_with();
    void test_join();
    void test_split();
    void test_rsplit();

private:
};


#endif  // LIBDNF_TEST_UTILS_STRING_HPP
