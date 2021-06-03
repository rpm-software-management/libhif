// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "test_vars.hpp"


CPPUNIT_TEST_SUITE_REGISTRATION(VarsTest);

// TODO possibly test the automatically detected vars (they depend on the host)

void VarsTest::test_vars() {
    vars.load("/", {PROJECT_SOURCE_DIR "/test/libdnf/conf/data/vars"});

    CPPUNIT_ASSERT_EQUAL(std::string("foovalue123-bar"), vars.substitute("foo$var1-bar"));
    CPPUNIT_ASSERT_EQUAL(std::string("$$$value123456-$nn-${nnn}"), vars.substitute("$$$${var1}$var2-$nn-${nnn}"));
}

void VarsTest::test_vars_multiple_dirs() {
    vars.load("/", {
        PROJECT_SOURCE_DIR "/test/libdnf/conf/data/vars",
        PROJECT_SOURCE_DIR "/test/libdnf/conf/data/vars2",
    });

    CPPUNIT_ASSERT_EQUAL(std::string("av333bthe answer is here"), vars.substitute("a${var1}b${var42}"));
}

void VarsTest::test_vars_env() {
    // Setting environment variables.
    // Environment variables have higher priority than variables from files.
    setenv("DNF0", "foo0", 1);
    setenv("DNF1", "foo1", 1);
    setenv("DNF9", "foo9", 1);
    setenv("DNF_VAR_var1", "testvar1", 1);
    setenv("DNF_VAR_var41", "testvar2", 1);

    // Load all variables.
    vars.load("/", {PROJECT_SOURCE_DIR "/test/libdnf/conf/data/vars"});

    // The variables var1 and var2 are defined in the files.
    // However, var1 was also an environment variable. The environment has a higher priority.
    CPPUNIT_ASSERT_EQUAL(
        std::string("foo0-foo1-foo9-testvar1-testvar2-456"),
        vars.substitute("${DNF0}-${DNF1}-${DNF9}-${var1}-${var41}-${var2}")
    );
}
