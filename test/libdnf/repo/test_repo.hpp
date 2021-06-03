// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_TEST_REPO_REPO_HPP
#define LIBDNF_TEST_REPO_REPO_HPP

#include "../testcase_fixture.hpp"

#include "libdnf/utils/temp.hpp"

#include <cppunit/extensions/HelperMacros.h>


class RepoTest : public TestCaseFixture {
    CPPUNIT_TEST_SUITE(RepoTest);
    CPPUNIT_TEST(test_repo_basics);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() override;
    void tearDown() override;

    void test_repo_basics();

private:
    libdnf::utils::TempDir * temp;
};

#endif
