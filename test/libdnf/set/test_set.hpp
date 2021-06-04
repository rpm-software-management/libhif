// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_TEST_SET_HPP
#define LIBDNF_TEST_SET_HPP


#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>


class SetTest : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE(SetTest);
    CPPUNIT_TEST(test_set_basics);
    CPPUNIT_TEST(test_set_equal_operator);
    CPPUNIT_TEST(test_set_assignment_operator);
    CPPUNIT_TEST(test_set_unary_operators);
    CPPUNIT_TEST(test_set_unary_operators);
    CPPUNIT_TEST(test_set_binary_operators);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() override;
    void tearDown() override;

    void test_set_basics();
    void test_set_equal_operator();
    void test_set_assignment_operator();
    void test_set_unary_operators();
    void test_set_unary_methods();
    void test_set_binary_operators();

private:
};

#endif
