// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef TEST_LIBDNF_RPM_RELDEP_LIST_HPP
#define TEST_LIBDNF_RPM_RELDEP_LIST_HPP

#include "../repo_fixture.hpp"

#include "libdnf/base/base.hpp"
#include "libdnf/rpm/package_sack.hpp"

#include <cppunit/extensions/HelperMacros.h>


class ReldepListTest : public RepoFixture {
    CPPUNIT_TEST_SUITE(ReldepListTest);
    CPPUNIT_TEST(test_get);
    CPPUNIT_TEST(test_add);
    CPPUNIT_TEST(test_size);
    CPPUNIT_TEST(test_compare);
    CPPUNIT_TEST(test_append);
    CPPUNIT_TEST(test_iterator);
    CPPUNIT_TEST(test_add_reldep_with_glob);
    CPPUNIT_TEST_SUITE_END();

public:
    void test_get();
    void test_add();
    void test_size();
    void test_compare();
    void test_append();
    void test_iterator();
    void test_add_reldep_with_glob();
};

#endif  // TEST_LIBDNF_RPM_RELDEP_LIST_HPP
