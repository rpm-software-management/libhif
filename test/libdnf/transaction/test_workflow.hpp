// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef TEST_LIBDNF_TRANSACTION_TEST_WORKFLOW_HPP
#define TEST_LIBDNF_TRANSACTION_TEST_WORKFLOW_HPP


#include "transaction_test_base.hpp"

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>


class TransactionWorkflowTest : public TransactionTestBase {
    CPPUNIT_TEST_SUITE(TransactionWorkflowTest);
    CPPUNIT_TEST(test_default_workflow);
    CPPUNIT_TEST_SUITE_END();

public:
    void test_default_workflow();
};


#endif  // TEST_LIBDNF_TRANSACTION_TEST_WORKFLOW_HPP
