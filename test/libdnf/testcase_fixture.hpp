// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef TEST_LIBDNF_TESTCASE_FIXTURE_HPP
#define TEST_LIBDNF_TESTCASE_FIXTURE_HPP


#include <cppunit/TestCase.h>

class TestCaseFixture : public CppUnit::TestCase {
public:
    void setUp() override;
    void tearDown() override;
};


#endif  // TEST_LIBDNF_TESTCASE_FIXTURE_HPP
