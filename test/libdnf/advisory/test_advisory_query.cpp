// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "test_advisory_query.hpp"

#include "test/libdnf/utils.hpp"

#include "libdnf/rpm/package_query.hpp"
#include "libdnf/rpm/package_set.hpp"

#include <filesystem>
#include <set>
#include <vector>


CPPUNIT_TEST_SUITE_REGISTRATION(AdvisoryAdvisoryQueryTest);

void AdvisoryAdvisoryQueryTest::setUp() {
    RepoFixture::setUp();
    RepoFixture::add_repo_repomd("repomd-repo1");
    advisory_sack = base->get_rpm_advisory_sack();
}

void AdvisoryAdvisoryQueryTest::test_size() {
    auto adv_query = advisory_sack->new_query();
    std::vector<std::string> expected = {"DNF-2019-1", "DNF-2020-1", "PKG-NEWER", "PKG-OLDER"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));
}

void AdvisoryAdvisoryQueryTest::test_filter_name() {
    // Tests filter_name method
    libdnf::advisory::AdvisoryQuery adv_query =
        advisory_sack->new_query().filter_name("*2020-1", libdnf::sack::QueryCmp::GLOB);
    std::vector<std::string> expected = {"DNF-2020-1"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query = advisory_sack->new_query().filter_name("DNF-20*", libdnf::sack::QueryCmp::GLOB);
    expected = {"DNF-2019-1", "DNF-2020-1"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query = advisory_sack->new_query().filter_name(
        std::vector<std::string>{"DNF-2019-1", "DNF-2020-1"}, libdnf::sack::QueryCmp::EQ);
    expected = {"DNF-2019-1", "DNF-2020-1"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));
}

void AdvisoryAdvisoryQueryTest::test_filter_type() {
    // Tests filter_type method
    libdnf::advisory::AdvisoryQuery adv_query =
        advisory_sack->new_query().filter_type(libdnf::advisory::Advisory::Type::BUGFIX);
    std::vector<std::string> expected = {"DNF-2020-1", "PKG-OLDER"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query = advisory_sack->new_query().filter_type(libdnf::advisory::Advisory::Type::ENHANCEMENT);
    expected = {"PKG-NEWER"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query = advisory_sack->new_query().filter_type(
        std::vector<libdnf::advisory::Advisory::Type>{
            libdnf::advisory::Advisory::Type::BUGFIX, libdnf::advisory::Advisory::Type::SECURITY},
        libdnf::sack::QueryCmp::EQ);
    expected = {"DNF-2019-1", "DNF-2020-1", "PKG-OLDER"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));
}

void AdvisoryAdvisoryQueryTest::test_filter_packages() {
    // Tests filter_packages method
    libdnf::rpm::PackageQuery pkg_query(sack);

    libdnf::advisory::AdvisoryQuery adv_query =
        advisory_sack->new_query().filter_packages(pkg_query, libdnf::sack::QueryCmp::GT);
    std::vector<std::string> expected = {"PKG-NEWER"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query = advisory_sack->new_query().filter_packages(pkg_query, libdnf::sack::QueryCmp::GTE);
    expected = {"DNF-2019-1", "PKG-NEWER"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query = advisory_sack->new_query().filter_packages(pkg_query, libdnf::sack::QueryCmp::EQ);
    expected = {"DNF-2019-1"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query = advisory_sack->new_query().filter_packages(pkg_query, libdnf::sack::QueryCmp::LTE);
    expected = {"DNF-2019-1", "PKG-OLDER"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query = advisory_sack->new_query().filter_packages(pkg_query, libdnf::sack::QueryCmp::LT);
    expected = {"PKG-OLDER"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));
}

void AdvisoryAdvisoryQueryTest::test_filter_cve() {
    // Tests filter_cve method
    libdnf::advisory::AdvisoryQuery adv_query =
        advisory_sack->new_query().filter_CVE("3333", libdnf::sack::QueryCmp::EQ);
    std::vector<std::string> expected = {"DNF-2020-1"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query =
        advisory_sack->new_query().filter_CVE(std::vector<std::string>{"1111", "3333"}, libdnf::sack::QueryCmp::EQ);
    expected = {"DNF-2019-1", "DNF-2020-1"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query =
        advisory_sack->new_query().filter_CVE(std::vector<std::string>{"1111", "4444"}, libdnf::sack::QueryCmp::EQ);
    expected = {"DNF-2019-1"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query = advisory_sack->new_query().filter_CVE("*", libdnf::sack::QueryCmp::GLOB);
    expected = {"DNF-2019-1", "DNF-2020-1"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));
}

void AdvisoryAdvisoryQueryTest::test_filter_bug() {
    // Tests filter_bug method
    libdnf::advisory::AdvisoryQuery adv_query =
        advisory_sack->new_query().filter_bug("2222", libdnf::sack::QueryCmp::EQ);
    std::vector<std::string> expected = {"DNF-2020-1"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query =
        advisory_sack->new_query().filter_bug(std::vector<std::string>{"1111", "3333"}, libdnf::sack::QueryCmp::EQ);
    expected = {};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query = advisory_sack->new_query().filter_bug("*", libdnf::sack::QueryCmp::GLOB);
    expected = {"DNF-2020-1"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));
}

void AdvisoryAdvisoryQueryTest::test_filter_severity() {
    // Tests filter_severity method
    libdnf::advisory::AdvisoryQuery adv_query =
        advisory_sack->new_query().filter_severity("moderate", libdnf::sack::QueryCmp::EQ);
    std::vector<std::string> expected = {"DNF-2019-1"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query = advisory_sack->new_query().filter_severity(
        std::vector<std::string>{"moderate", "critical"}, libdnf::sack::QueryCmp::EQ);
    expected = {"DNF-2019-1", "DNF-2020-1"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));
}

void AdvisoryAdvisoryQueryTest::test_get_sorted_advisory_packages() {
    // Tests get_sorted_advisory_packages method
    std::vector<libdnf::advisory::AdvisoryPackage> adv_pkgs = advisory_sack->new_query().get_sorted_advisory_packages();
    CPPUNIT_ASSERT_EQUAL(7lu, adv_pkgs.size());
    CPPUNIT_ASSERT_EQUAL(std::string("pkg"), adv_pkgs[0].get_name());
    CPPUNIT_ASSERT_EQUAL(std::string("1.2-3"), adv_pkgs[0].get_evr());
    CPPUNIT_ASSERT_EQUAL(std::string("pkg"), adv_pkgs[1].get_name());
    CPPUNIT_ASSERT_EQUAL(std::string("0.1-1"), adv_pkgs[1].get_evr());
    CPPUNIT_ASSERT_EQUAL(std::string("pkg"), adv_pkgs[2].get_name());
    CPPUNIT_ASSERT_EQUAL(std::string("4.0-1"), adv_pkgs[2].get_evr());
    CPPUNIT_ASSERT_EQUAL(std::string("bitcoin"), adv_pkgs[3].get_name());
    CPPUNIT_ASSERT_EQUAL(std::string("2.5-1"), adv_pkgs[3].get_evr());
    CPPUNIT_ASSERT_EQUAL(std::string("filesystem"), adv_pkgs[4].get_name());
    CPPUNIT_ASSERT_EQUAL(std::string("3.9-2.fc29"), adv_pkgs[4].get_evr());
    CPPUNIT_ASSERT_EQUAL(std::string("wget"), adv_pkgs[5].get_name());
    CPPUNIT_ASSERT_EQUAL(std::string("1.19.5-5.fc29"), adv_pkgs[5].get_evr());
    CPPUNIT_ASSERT_EQUAL(std::string("yum"), adv_pkgs[6].get_name());
    CPPUNIT_ASSERT_EQUAL(std::string("3.4.3-0"), adv_pkgs[6].get_evr());

    adv_pkgs = advisory_sack->new_query().filter_name("DNF-2020-1").get_sorted_advisory_packages();
    CPPUNIT_ASSERT_EQUAL(3lu, adv_pkgs.size());
    CPPUNIT_ASSERT_EQUAL(std::string("bitcoin"), adv_pkgs[0].get_name());
    CPPUNIT_ASSERT_EQUAL(std::string("2.5-1"), adv_pkgs[0].get_evr());
    CPPUNIT_ASSERT_EQUAL(std::string("wget"), adv_pkgs[1].get_name());
    CPPUNIT_ASSERT_EQUAL(std::string("1.19.5-5.fc29"), adv_pkgs[1].get_evr());
    CPPUNIT_ASSERT_EQUAL(std::string("yum"), adv_pkgs[2].get_name());
    CPPUNIT_ASSERT_EQUAL(std::string("3.4.3-0"), adv_pkgs[2].get_evr());
}
