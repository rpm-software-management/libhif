// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "test_fs.hpp"

#include <filesystem>

#include "libdnf/utils/fs.hpp"


using namespace libdnf::utils::fs;


CPPUNIT_TEST_SUITE_REGISTRATION(UtilsFsTest);


void UtilsFsTest::setUp() {
    temp = new libdnf::utils::TempDir("libdnf_unittest_", {"already-exists"});
}


void UtilsFsTest::tearDown() {
    delete temp;
}


void UtilsFsTest::test_makedirs_for_file() {
    // create a directory that doesn't exist
    auto does_not_exist = temp->get_path() / "does-not-exist";
    CPPUNIT_ASSERT_EQUAL(false, std::filesystem::exists(does_not_exist));
    makedirs_for_file(does_not_exist / "");
    CPPUNIT_ASSERT_EQUAL(true, std::filesystem::exists(does_not_exist));

    // don't fail on a directory that already exists
    auto already_exists = temp->get_path() / "already-exists";
    CPPUNIT_ASSERT_EQUAL(true, std::filesystem::exists(already_exists));
    makedirs_for_file(already_exists / "");
    CPPUNIT_ASSERT_EQUAL(true, std::filesystem::exists(already_exists));

    // create subdirs
    auto dir = temp->get_path() / "one/two/three";
    CPPUNIT_ASSERT_EQUAL(false, std::filesystem::exists(dir));
    makedirs_for_file(dir / "");
    CPPUNIT_ASSERT_EQUAL(true, std::filesystem::exists(dir));
}
