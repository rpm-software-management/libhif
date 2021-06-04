// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_TEST_REPO_QUERY_HPP
#define LIBDNF_TEST_REPO_QUERY_HPP


#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>


class RepoQueryTest : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE(RepoQueryTest);
    CPPUNIT_TEST(test_query_basics);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() override;
    void tearDown() override;

    void test_query_basics();
};

#endif
