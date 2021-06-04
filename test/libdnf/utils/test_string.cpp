// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "test_string.hpp"

#include "test/libdnf/utils.hpp"

#include "libdnf/utils/string.hpp"


using namespace libdnf::utils::string;


CPPUNIT_TEST_SUITE_REGISTRATION(UtilsStringTest);


void UtilsStringTest::setUp() {}


void UtilsStringTest::tearDown() {}


void UtilsStringTest::test_starts_with() {
    CPPUNIT_ASSERT_EQUAL(true, starts_with("", ""));
    CPPUNIT_ASSERT_EQUAL(true, starts_with("abc", ""));
    CPPUNIT_ASSERT_EQUAL(true, starts_with("abc", "a"));
    CPPUNIT_ASSERT_EQUAL(true, starts_with("abc", "ab"));
    CPPUNIT_ASSERT_EQUAL(true, starts_with("abc", "abc"));
    CPPUNIT_ASSERT_EQUAL(false, starts_with("abc", "abcd"));
    CPPUNIT_ASSERT_EQUAL(false, starts_with("abc", "b"));
}


void UtilsStringTest::test_ends_with() {
    CPPUNIT_ASSERT_EQUAL(true, ends_with("", ""));
    CPPUNIT_ASSERT_EQUAL(true, ends_with("abc", ""));
    CPPUNIT_ASSERT_EQUAL(true, ends_with("abc", "c"));
    CPPUNIT_ASSERT_EQUAL(true, ends_with("abc", "bc"));
    CPPUNIT_ASSERT_EQUAL(true, ends_with("abc", "abc"));
    CPPUNIT_ASSERT_EQUAL(false, ends_with("abc", "0abc"));
    CPPUNIT_ASSERT_EQUAL(false, ends_with("abc", "b"));
}


void UtilsStringTest::test_join() {
    CPPUNIT_ASSERT_EQUAL(std::string(""), join(std::vector<std::string>(), ""));
    CPPUNIT_ASSERT_EQUAL(std::string(""), join(std::vector<std::string>(), "; "));
    CPPUNIT_ASSERT_EQUAL(std::string(""), join(std::vector<std::string>({""}), ""));
    CPPUNIT_ASSERT_EQUAL(std::string(""), join(std::vector<std::string>({""}), "; "));
    CPPUNIT_ASSERT_EQUAL(std::string("aa"), join(std::vector<std::string>({"aa"}), "; "));
    CPPUNIT_ASSERT_EQUAL(std::string("aa; bb"), join(std::vector<std::string>({"aa", "bb"}), "; "));
}


void UtilsStringTest::test_split() {
    CPPUNIT_ASSERT_EQUAL(std::vector<std::string>({""}), split("", "; "));
    CPPUNIT_ASSERT_EQUAL(std::vector<std::string>({"aa"}), split("aa", "; "));
    CPPUNIT_ASSERT_EQUAL(std::vector<std::string>({"aa", "bb"}), split("aa; bb", "; "));
    CPPUNIT_ASSERT_EQUAL(std::vector<std::string>({"aa; bb; cc"}), split("aa; bb; cc", "; ", 0));
    CPPUNIT_ASSERT_EQUAL(std::vector<std::string>({"aa; bb; cc"}), split("aa; bb; cc", "; ", 1));
    CPPUNIT_ASSERT_EQUAL(std::vector<std::string>({"aa", "bb; cc"}), split("aa; bb; cc", "; ", 2));
    CPPUNIT_ASSERT_EQUAL(std::vector<std::string>({"aa", "bb", "cc"}), split("aa; bb; cc", "; ", 3));
    CPPUNIT_ASSERT_EQUAL(std::vector<std::string>({"aa", "bb", "cc"}), split("aa; bb; cc", "; ", 4));
    CPPUNIT_ASSERT_EQUAL(std::vector<std::string>({"aa", "bb", "cc", ""}), split("aa; bb; cc; ", "; ", 4));
}


void UtilsStringTest::test_rsplit() {
    CPPUNIT_ASSERT_EQUAL(std::vector<std::string>({"aa; bb; cc"}), rsplit("aa; bb; cc", "; ", 0));
    CPPUNIT_ASSERT_EQUAL(std::vector<std::string>({"aa; bb; cc"}), rsplit("aa; bb; cc", "; ", 1));
    CPPUNIT_ASSERT_EQUAL(std::vector<std::string>({"aa; bb", "cc"}), rsplit("aa; bb; cc", "; ", 2));
    CPPUNIT_ASSERT_EQUAL(std::vector<std::string>({"aa", "bb", "cc"}), rsplit("aa; bb; cc", "; ", 3));
    CPPUNIT_ASSERT_EQUAL(std::vector<std::string>({"aa", "bb", "cc"}), rsplit("aa; bb; cc", "; ", 4));
    CPPUNIT_ASSERT_EQUAL(std::vector<std::string>({"aa", "bb", "cc", ""}), rsplit("aa; bb; cc; ", "; ", 4));
}
