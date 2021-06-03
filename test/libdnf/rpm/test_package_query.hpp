// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef TEST_LIBDNF_RPM_PACKAGE_QUERY_HPP
#define TEST_LIBDNF_RPM_PACKAGE_QUERY_HPP


#include "../repo_fixture.hpp"

#include <cppunit/extensions/HelperMacros.h>


class RpmPackageQueryTest : public RepoFixture {
    CPPUNIT_TEST_SUITE(RpmPackageQueryTest);

#ifndef WITH_PERFORMANCE_TESTS
    CPPUNIT_TEST(test_size);
    CPPUNIT_TEST(test_filter_latest);
    CPPUNIT_TEST(test_filter_name);
    CPPUNIT_TEST(test_filter_name_packgset);
    CPPUNIT_TEST(test_filter_nevra_packgset);
    CPPUNIT_TEST(test_filter_name_arch);
    CPPUNIT_TEST(test_filter_name_arch2);
    CPPUNIT_TEST(test_filter_nevra);
    CPPUNIT_TEST(test_filter_version);
    CPPUNIT_TEST(test_filter_release);
    CPPUNIT_TEST(test_filter_priority);
    CPPUNIT_TEST(test_filter_provides);
    CPPUNIT_TEST(test_filter_requires);
    CPPUNIT_TEST(test_filter_advisories);
    CPPUNIT_TEST(test_filter_chain);
    CPPUNIT_TEST(test_resolve_pkg_spec);
    CPPUNIT_TEST(test_update);
    CPPUNIT_TEST(test_intersection);
    CPPUNIT_TEST(test_difference);
#endif

#ifdef WITH_PERFORMANCE_TESTS
#endif

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() override;

    void test_size();
    void test_filter_latest();
    void test_filter_name();
    void test_filter_name_packgset();
    void test_filter_nevra_packgset();
    void test_filter_name_arch();
    void test_filter_name_arch2();
    void test_filter_nevra();
    void test_filter_version();
    void test_filter_release();
    void test_filter_provides();
    void test_filter_priority();
    void test_filter_requires();
    void test_filter_advisories();
    void test_filter_chain();
    void test_resolve_pkg_spec();
    void test_update();
    void test_intersection();
    void test_difference();
    // TODO(jmracek) Add tests when system repo will be available
    // PackageQuery & filter_upgrades();
    // PackageQuery & filter_downgrades();
    // PackageQuery & filter_upgradable();
    // PackageQuery & filter_downgradable();
};


#endif  // TEST_LIBDNF_RPM_PACKAGE_QUERY_HPP
