// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef TEST_LIBDNF_ADVISORY_ADVISORY_HPP
#define TEST_LIBDNF_ADVISORY_ADVISORY_HPP


#include "../repo_fixture.hpp"

#include "libdnf/advisory/advisory_collection.hpp"
#include "libdnf/advisory/advisory_sack.hpp"

#include <cppunit/extensions/HelperMacros.h>

class AdvisoryAdvisoryTest : public RepoFixture {
    CPPUNIT_TEST_SUITE(AdvisoryAdvisoryTest);

    CPPUNIT_TEST(test_get_name);
    CPPUNIT_TEST(test_get_type);
    CPPUNIT_TEST(test_get_type_cstring);
    CPPUNIT_TEST(test_get_severity);
    CPPUNIT_TEST(test_get_references);
    CPPUNIT_TEST(test_get_collections);

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() override;

    void test_get_name();
    void test_get_type();
    void test_get_type_cstring();
    void test_get_severity();
    //void test_filter_package();

    void test_get_references();
    void test_get_collections();

private:
    libdnf::advisory::AdvisorySackWeakPtr advisory_sack;
    std::vector<libdnf::advisory::Advisory> advisories;
    libdnf::advisory::Advisory * advisory;
};


#endif  // TEST_LIBDNF_ADVISORY_ADVISORY_HPP
