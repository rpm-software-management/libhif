// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef TEST_LIBDNF_TRANSACTION_TEST_COMPS_GROUP_HPP
#define TEST_LIBDNF_TRANSACTION_TEST_COMPS_GROUP_HPP


#include "transaction_test_base.hpp"

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>


class TransactionCompsGroupTest : public TransactionTestBase {
    CPPUNIT_TEST_SUITE(TransactionCompsGroupTest);
    CPPUNIT_TEST(test_save_load);
    CPPUNIT_TEST_SUITE_END();

public:
    void test_save_load();
};


#endif  // TEST_LIBDNF_TRANSACTION_TEST_COMPS_GROUP_HPP
