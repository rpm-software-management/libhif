
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_TEST_RELDEP_HPP
#define LIBDNF_TEST_RELDEP_HPP

#include "libdnf/base/base.hpp"
#include "libdnf/rpm/package_sack.hpp"

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>


class ReldepTest : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE(ReldepTest);
    CPPUNIT_TEST(test_short_reldep);
    CPPUNIT_TEST(test_full_reldep);
    CPPUNIT_TEST(test_rich_reldep);
    CPPUNIT_TEST(test_invalid_reldep);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() override;

    void test_short_reldep();
    void test_full_reldep();
    void test_rich_reldep();
    void test_invalid_reldep();

private:
    std::unique_ptr<libdnf::Base> base;
    libdnf::rpm::PackageSackWeakPtr sack;
};

#endif  // TEST_LIBDNF_RPM_RELDEP_HPP
