// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef TEST_LIBDNF_ADVISORY_ADVISORY_QUERY_HPP
#define TEST_LIBDNF_ADVISORY_ADVISORY_QUERY_HPP


#include "../repo_fixture.hpp"

#include "libdnf/advisory/advisory_sack.hpp"

#include <cppunit/extensions/HelperMacros.h>

class AdvisoryAdvisoryQueryTest : public RepoFixture {
    CPPUNIT_TEST_SUITE(AdvisoryAdvisoryQueryTest);

    CPPUNIT_TEST(test_size);
    CPPUNIT_TEST(test_filter_name);
    CPPUNIT_TEST(test_filter_type);
    CPPUNIT_TEST(test_filter_packages);
    CPPUNIT_TEST(test_filter_cve);
    CPPUNIT_TEST(test_filter_bug);
    CPPUNIT_TEST(test_filter_severity);
    CPPUNIT_TEST(test_get_sorted_advisory_packages);

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() override;

    void test_size();
    void test_filter_name();
    void test_filter_type();
    void test_filter_packages();
    void test_filter_cve();
    void test_filter_bug();
    void test_filter_severity();
    void test_get_sorted_advisory_packages();

private:
    libdnf::advisory::AdvisorySackWeakPtr advisory_sack;
};


#endif  // TEST_LIBDNF_ADVISORY_ADVISORY_QUERY_HPP
