// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef TEST_LIBDNF_RPM_PACKAGE_SET_HPP
#define TEST_LIBDNF_RPM_PACKAGE_SET_HPP


#include "../repo_fixture.hpp"

#include "libdnf/rpm/package_set.hpp"

#include <cppunit/extensions/HelperMacros.h>

#include <memory>


class RpmPackageSetTest : public RepoFixture {
    CPPUNIT_TEST_SUITE(RpmPackageSetTest);

#ifndef WITH_PERFORMANCE_TESTS
    CPPUNIT_TEST(test_add);
    CPPUNIT_TEST(test_contains);
    CPPUNIT_TEST(test_remove);
    CPPUNIT_TEST(test_union);
    CPPUNIT_TEST(test_intersection);
    CPPUNIT_TEST(test_difference);
    CPPUNIT_TEST(test_iterator);
#endif

#ifdef WITH_PERFORMANCE_TESTS
#endif

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() override;

    void test_add();
    void test_contains();
    void test_remove();

    void test_union();
    void test_intersection();
    void test_difference();

    void test_iterator();


private:
    std::unique_ptr<libdnf::rpm::PackageSet> set1;
    std::unique_ptr<libdnf::rpm::PackageSet> set2;
};


#endif  // TEST_LIBDNF_RPM_PACKAGE_SET_HPP
