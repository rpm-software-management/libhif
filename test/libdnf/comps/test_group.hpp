// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_TEST_COMPS_GROUP_HPP
#define LIBDNF_TEST_COMPS_GROUP_HPP


#include "libdnf/base/base.hpp"

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>


class CompsGroupTest : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE(CompsGroupTest);
    CPPUNIT_TEST(test_load);
    CPPUNIT_TEST(test_load_defaults);
    CPPUNIT_TEST(test_merge);
    CPPUNIT_TEST(test_merge_with_empty);
    CPPUNIT_TEST(test_merge_empty_with_nonempty);
    CPPUNIT_TEST(test_dump_and_load);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() override;
    void tearDown() override;

    void test_load();
    void test_load_defaults();
    void test_merge();
    void test_merge_with_empty();
    void test_merge_empty_with_nonempty();
    void test_dump_and_load();

protected:
    std::unique_ptr<libdnf::Base> base;
};

#endif
