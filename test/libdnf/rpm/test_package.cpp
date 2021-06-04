// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "test_package.hpp"

#include "../utils.hpp"

#include "libdnf/rpm/nevra.hpp"
#include "libdnf/rpm/package_query.hpp"

#include <vector>


CPPUNIT_TEST_SUITE_REGISTRATION(RpmPackageTest);


void RpmPackageTest::setUp() {
    RepoFixture::setUp();
    add_repo_repomd("repomd-repo1");
}


libdnf::rpm::Package RpmPackageTest::get_pkg(const std::string & nevra) {
    libdnf::rpm::PackageQuery query(sack);
    query.filter_nevra({nevra});
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "get_pkg(\"" + nevra + "\"): no package or more than one package found.", 1lu, query.size());
    return *query.begin();
}


void RpmPackageTest::test_equality() {
    auto package1 = get_pkg("pkg-1.2-3.x86_64");
    auto package2 = get_pkg("pkg-1.2-3.x86_64");
    auto package3 = get_pkg("pkg-libs-1:1.3-4.x86_64");

    CPPUNIT_ASSERT(package1 == package2);
    CPPUNIT_ASSERT(!(package1 == package3));

    CPPUNIT_ASSERT(package1 != package3);
    CPPUNIT_ASSERT(!(package1 != package2));
}


void RpmPackageTest::test_get_id() {
    CPPUNIT_ASSERT(get_pkg("pkg-1.2-3.x86_64").get_id().id != 0);
}


void RpmPackageTest::test_get_name() {
    CPPUNIT_ASSERT_EQUAL(std::string("pkg"), get_pkg("pkg-1.2-3.x86_64").get_name());
}


void RpmPackageTest::test_get_epoch() {
    CPPUNIT_ASSERT_EQUAL(std::string("0"), get_pkg("pkg-1.2-3.x86_64").get_epoch());
    CPPUNIT_ASSERT_EQUAL(std::string("1"), get_pkg("pkg-libs-1:1.3-4.x86_64").get_epoch());
}


void RpmPackageTest::test_get_version() {
    CPPUNIT_ASSERT_EQUAL(std::string("1.2"), get_pkg("pkg-1.2-3.x86_64").get_version());
}


void RpmPackageTest::test_get_release() {
    CPPUNIT_ASSERT_EQUAL(std::string("3"), get_pkg("pkg-1.2-3.x86_64").get_release());
}


void RpmPackageTest::test_get_arch() {
    CPPUNIT_ASSERT_EQUAL(std::string("x86_64"), get_pkg("pkg-1.2-3.x86_64").get_arch());
}


void RpmPackageTest::test_get_evr() {
    CPPUNIT_ASSERT_EQUAL(std::string("1.2-3"), get_pkg("pkg-1.2-3.x86_64").get_evr());
}


void RpmPackageTest::test_get_nevra() {
    CPPUNIT_ASSERT_EQUAL(std::string("pkg-1.2-3.x86_64"), get_pkg("pkg-1.2-3.x86_64").get_nevra());
}


void RpmPackageTest::test_get_full_nevra() {
    CPPUNIT_ASSERT_EQUAL(std::string("pkg-0:1.2-3.x86_64"), get_pkg("pkg-1.2-3.x86_64").get_full_nevra());
}


void RpmPackageTest::test_get_group() {
    CPPUNIT_ASSERT_EQUAL(std::string("Group"), get_pkg("pkg-1.2-3.x86_64").get_group());
}


void RpmPackageTest::test_get_package_size() {
    CPPUNIT_ASSERT_EQUAL(111LLU, get_pkg("pkg-1.2-3.x86_64").get_package_size());
}


void RpmPackageTest::test_get_install_size() {
    // TODO implement installing packages for tests
    CPPUNIT_ASSERT_EQUAL(222LLU, get_pkg("pkg-1.2-3.x86_64").get_install_size());
}


void RpmPackageTest::test_get_license() {
    CPPUNIT_ASSERT_EQUAL(std::string("License"), get_pkg("pkg-1.2-3.x86_64").get_license());
}


void RpmPackageTest::test_get_sourcerpm() {
    CPPUNIT_ASSERT_EQUAL(std::string("pkg-1.2-3.src.rpm"), get_pkg("pkg-1.2-3.x86_64").get_sourcerpm());
}


void RpmPackageTest::test_get_build_time() {
    CPPUNIT_ASSERT_EQUAL(456LLU, get_pkg("pkg-1.2-3.x86_64").get_build_time());
}


// TODO not supported by libsolv: https://github.com/openSUSE/libsolv/issues/400
// void RpmPackageTest::test_get_build_host() {
//     CPPUNIT_ASSERT_EQUAL(std::string("Buildhost"), get_pkg("pkg-1.2-3.x86_64").get_build_host());
// }


void RpmPackageTest::test_get_packager() {
    CPPUNIT_ASSERT_EQUAL(std::string("Packager"), get_pkg("pkg-1.2-3.x86_64").get_packager());
}


void RpmPackageTest::test_get_vendor() {
    CPPUNIT_ASSERT_EQUAL(std::string("Vendor"), get_pkg("pkg-1.2-3.x86_64").get_vendor());
}


void RpmPackageTest::test_get_url() {
    CPPUNIT_ASSERT_EQUAL(std::string("http://example.com/"), get_pkg("pkg-1.2-3.x86_64").get_url());
}


void RpmPackageTest::test_get_summary() {
    CPPUNIT_ASSERT_EQUAL(std::string("Summary"), get_pkg("pkg-1.2-3.x86_64").get_summary());
}


void RpmPackageTest::test_get_description() {
    CPPUNIT_ASSERT_EQUAL(std::string("Description"), get_pkg("pkg-1.2-3.x86_64").get_description());
}


void RpmPackageTest::test_get_files() {
    const auto files = get_pkg("pkg-1.2-3.x86_64").get_files();
    const std::vector<std::string> expected = {
        "/etc/pkg.conf",
        "/etc/pkg.conf.d",
    };
    CPPUNIT_ASSERT_EQUAL(expected, files);
}


void RpmPackageTest::test_get_provides() {
    const auto actual = get_pkg("pkg-1.2-3.x86_64").get_provides();
    const std::vector<std::string> expected = {"pkg = 1.2-3"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_string(actual));
}


void RpmPackageTest::test_get_requires() {
    const auto actual = get_pkg("unresolvable-1:2-3.noarch").get_requires();
    const std::vector<std::string> expected = {"req = 1:2-3", "prereq"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_string(actual));
}


void RpmPackageTest::test_get_requires_pre() {
    const auto actual = get_pkg("unresolvable-1:2-3.noarch").get_requires_pre();
    const std::vector<std::string> expected = {"prereq"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_string(actual));
}


void RpmPackageTest::test_get_conflicts() {
    const auto actual = get_pkg("unresolvable-1:2-3.noarch").get_conflicts();
    const std::vector<std::string> expected = {"con < 1:2"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_string(actual));
}


void RpmPackageTest::test_get_obsoletes() {
    const auto actual = get_pkg("unresolvable-1:2-3.noarch").get_obsoletes();
    const std::vector<std::string> expected = {"obs < 1:2"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_string(actual));
}


void RpmPackageTest::test_get_prereq_ignoreinst() {
    const auto actual = get_pkg("unresolvable-1:2-3.noarch").get_prereq_ignoreinst();
    // TODO requires the package to be installed
    //const std::vector<std::string> expected = {"test-requires-pre"};
    const std::vector<std::string> expected = {};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_string(actual));
}


void RpmPackageTest::test_get_regular_requires() {
    const auto actual = get_pkg("unresolvable-1:2-3.noarch").get_regular_requires();
    const std::vector<std::string> expected = {"req = 1:2-3"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_string(actual));
}


void RpmPackageTest::test_get_recommends() {
    const auto actual = get_pkg("unresolvable-1:2-3.noarch").get_recommends();
    const std::vector<std::string> expected = {"rec"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_string(actual));
}


void RpmPackageTest::test_get_suggests() {
    const auto suggests = get_pkg("unresolvable-1:2-3.noarch").get_suggests();
    const std::vector<std::string> expected = {"sug"};

    CPPUNIT_ASSERT_EQUAL(expected, to_vector_string(suggests));
}


void RpmPackageTest::test_get_enhances() {
    const auto enhances = get_pkg("unresolvable-1:2-3.noarch").get_enhances();
    const std::vector<std::string> expected = {"enh"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_string(enhances));
}


void RpmPackageTest::test_get_supplements() {
    const auto supplements = get_pkg("unresolvable-1:2-3.noarch").get_supplements();
    const std::vector<std::string> expected = {"sup"};

    CPPUNIT_ASSERT_EQUAL(expected, to_vector_string(supplements));
}


void RpmPackageTest::test_get_baseurl() {
    CPPUNIT_ASSERT_EQUAL(std::string(), get_pkg("pkg-1.2-3.x86_64").get_baseurl());
    CPPUNIT_ASSERT_EQUAL(std::string("http://example.com/"), get_pkg("unresolvable-1:2-3.noarch").get_baseurl());
}


void RpmPackageTest::test_get_location() {
    CPPUNIT_ASSERT_EQUAL(std::string("pkg-1.2-3.x86_64.rpm"), get_pkg("pkg-1.2-3.x86_64").get_location());
}


void RpmPackageTest::test_get_checksum() {
    CPPUNIT_ASSERT_EQUAL(
        std::string("ec57b154a186fdc1f71976fc0fde97d51c744bc88d222828b4cfa42e3b1f855b"),
        get_pkg("pkg-1.2-3.x86_64").get_checksum().get_checksum());
}


void RpmPackageTest::test_get_hdr_checksum() {
    // TODO header checksum is available only for installed packages
    CPPUNIT_ASSERT_EQUAL(std::string(""), get_pkg("pkg-1.2-3.x86_64").get_hdr_checksum().get_checksum());
}


void RpmPackageTest::test_is_installed() {
    // TODO implement installing packages for tests
    CPPUNIT_ASSERT_EQUAL(false, get_pkg("pkg-1.2-3.x86_64").is_installed());
}


void RpmPackageTest::test_get_hdr_end() {
    CPPUNIT_ASSERT_EQUAL(22LLU, get_pkg("pkg-1.2-3.x86_64").get_hdr_end());
}


void RpmPackageTest::test_get_install_time() {
    // TODO implement installing packages for tests
    CPPUNIT_ASSERT_EQUAL(0LLU, get_pkg("pkg-1.2-3.x86_64").get_install_time());
}


void RpmPackageTest::test_get_media_number() {
    // TODO zeros everywhere
    CPPUNIT_ASSERT_EQUAL(0LLU, get_pkg("pkg-1.2-3.x86_64").get_media_number());
}


void RpmPackageTest::test_get_rpmdbid() {
    // TODO zeros everywhere
    CPPUNIT_ASSERT_EQUAL(0LLU, get_pkg("pkg-1.2-3.x86_64").get_rpmdbid());
}


void RpmPackageTest::test_to_nevra_string() {
    // test that to_nevra_string() template function works
    auto pkg = get_pkg("pkg-1.2-3.x86_64");
    CPPUNIT_ASSERT_EQUAL(std::string("pkg-1.2-3.x86_64"), libdnf::rpm::to_nevra_string(pkg));

    auto pkg2 = get_pkg("pkg-libs-1:1.3-4.x86_64");
    CPPUNIT_ASSERT_EQUAL(std::string("pkg-libs-1:1.3-4.x86_64"), libdnf::rpm::to_nevra_string(pkg2));
}


void RpmPackageTest::test_to_full_nevra_string() {
    // test that to_full_nevra_string() template function works
    auto pkg = get_pkg("pkg-1.2-3.x86_64");
    CPPUNIT_ASSERT_EQUAL(std::string("pkg-0:1.2-3.x86_64"), libdnf::rpm::to_full_nevra_string(pkg));

    auto pkg2 = get_pkg("pkg-libs-1:1.3-4.x86_64");
    CPPUNIT_ASSERT_EQUAL(std::string("pkg-libs-1:1.3-4.x86_64"), libdnf::rpm::to_full_nevra_string(pkg2));
}
