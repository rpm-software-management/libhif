// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_TEST_WEAK_PTR_HPP
#define LIBDNF_TEST_WEAK_PTR_HPP


#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>


class WeakPtrTest : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE(WeakPtrTest);
    CPPUNIT_TEST(test_weak_ptr);
    CPPUNIT_TEST(test_weak_ptr_is_owner);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() override;
    void tearDown() override;

    void test_weak_ptr();
    void test_weak_ptr_is_owner();

private:
};

#endif
