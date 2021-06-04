// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef TEST_LIBDNF_TRANSACTION_TEST_TRANSACTION_HPP
#define TEST_LIBDNF_TRANSACTION_TEST_TRANSACTION_HPP


#include "transaction_test_base.hpp"

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>


class TransactionTest : public TransactionTestBase {
    CPPUNIT_TEST_SUITE(TransactionTest);
    CPPUNIT_TEST(test_save_load);
    CPPUNIT_TEST(test_save_with_specified_id_raises);
    CPPUNIT_TEST(test_second_start_raises);
    CPPUNIT_TEST(test_update);
    CPPUNIT_TEST(test_compare);
    CPPUNIT_TEST_SUITE_END();

public:
    void test_save_load();
    void test_save_with_specified_id_raises();
    void test_second_start_raises();
    void test_update();
    void test_compare();
};


#endif  // TEST_LIBDNF_TRANSACTION_TEST_TRANSACTION_HPP
