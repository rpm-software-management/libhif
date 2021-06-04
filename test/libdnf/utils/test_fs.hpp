// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_TEST_UTILS_FS_HPP
#define LIBDNF_TEST_UTILS_FS_HPP


#include "libdnf/utils/temp.hpp"

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>


class UtilsFsTest : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE(UtilsFsTest);
    CPPUNIT_TEST(test_makedirs_for_file);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() override;
    void tearDown() override;

    void test_makedirs_for_file();

private:
    libdnf::utils::TempDir * temp;
};


#endif  // LIBDNF_TEST_UTILS_FS_HPP
