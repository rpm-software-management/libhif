// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef TEST_LIBDNF_BASE_BASE_HPP
#define TEST_LIBDNF_BASE_BASE_HPP


#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>


class BaseTest : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE(BaseTest);
    CPPUNIT_TEST(test_weak_ptr);
    CPPUNIT_TEST_SUITE_END();

public:
    void test_weak_ptr();
};


#endif  // TEST_LIBDNF_BASE_GOAL_HPP
