// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef TEST_LIBDNF_ADVISORY_ADVISORY_MODULE_HPP
#define TEST_LIBDNF_ADVISORY_ADVISORY_MODULE_HPP


#include "../repo_fixture.hpp"

#include "libdnf/advisory/advisory_collection.hpp"
#include "libdnf/advisory/advisory_module.hpp"

#include <cppunit/extensions/HelperMacros.h>

class AdvisoryAdvisoryModuleTest : public RepoFixture {
    CPPUNIT_TEST_SUITE(AdvisoryAdvisoryModuleTest);

    CPPUNIT_TEST(test_get_name);
    CPPUNIT_TEST(test_get_stream);
    CPPUNIT_TEST(test_get_version);
    CPPUNIT_TEST(test_get_context);
    CPPUNIT_TEST(test_get_arch);

    CPPUNIT_TEST(test_get_advisory_id);
    CPPUNIT_TEST(test_get_advisory);
    CPPUNIT_TEST(test_get_advisory_collection);

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() override;

    void test_get_name();
    void test_get_stream();
    void test_get_version();
    void test_get_context();
    void test_get_arch();

    void test_get_advisory_id();
    void test_get_advisory();
    void test_get_advisory_collection();

private:
    std::vector<libdnf::advisory::AdvisoryModule> modules;
};


#endif  // TEST_LIBDNF_ADVISORY_ADVISORY_MODULE_HPP
