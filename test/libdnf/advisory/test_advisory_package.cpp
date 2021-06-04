// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "test_advisory_package.hpp"

#include <filesystem>
#include <set>
#include <vector>


CPPUNIT_TEST_SUITE_REGISTRATION(AdvisoryAdvisoryPackageTest);

//This allows running only this single test suite, by using `getRegistry("AdvisoryAdvisoryTest_suite")` in run_tests.cpp
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(AdvisoryAdvisoryPackageTest, "AdvisoryAdvisoryPackageTest_suite");

void AdvisoryAdvisoryPackageTest::setUp() {
    RepoFixture::setUp();
    RepoFixture::add_repo_repomd("repomd-repo1");
    auto advisory = base->get_rpm_advisory_sack()->new_query().filter_name("DNF-2019-1").get_advisories()[0];
    std::vector<libdnf::advisory::AdvisoryCollection> collections = advisory.get_collections();
    packages = collections[0].get_packages();
}

void AdvisoryAdvisoryPackageTest::test_get_name() {
    // Tests get_name method
    CPPUNIT_ASSERT_EQUAL(std::string("pkg"), std::string(packages[0].get_name()));
}

void AdvisoryAdvisoryPackageTest::test_get_version() {
    // Tests get_version method
    CPPUNIT_ASSERT_EQUAL(std::string("1.2"), std::string(packages[0].get_version()));
}

void AdvisoryAdvisoryPackageTest::test_get_evr() {
    // Tests get_evr method
    CPPUNIT_ASSERT_EQUAL(std::string("1.2-3"), std::string(packages[0].get_evr()));
}

void AdvisoryAdvisoryPackageTest::test_get_arch() {
    // Tests get_arch method
    CPPUNIT_ASSERT_EQUAL(std::string("x86_64"), std::string(packages[0].get_arch()));
}

void AdvisoryAdvisoryPackageTest::test_get_advisory_id() {
    // Tests get_advisory_id method
    libdnf::advisory::AdvisoryId adv_id = packages[0].get_advisory_id();
    CPPUNIT_ASSERT(adv_id.id > 0);
}

void AdvisoryAdvisoryPackageTest::test_get_advisory() {
    // Tests get_advisory method
    libdnf::advisory::Advisory a = packages[0].get_advisory();
    CPPUNIT_ASSERT_EQUAL(std::string("DNF-2019-1"), a.get_name());
}

void AdvisoryAdvisoryPackageTest::test_get_advisory_collection() {
    // Tests get_advisory_collection method
    libdnf::advisory::AdvisoryCollection ac = packages[0].get_advisory_collection();
    std::vector<libdnf::advisory::AdvisoryPackage> out_pkgs = ac.get_packages();
    size_t target_size = 2;
    CPPUNIT_ASSERT_EQUAL(target_size, out_pkgs.size());
}
