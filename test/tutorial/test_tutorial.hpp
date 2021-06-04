// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef TEST_TUTORIAL_TEST_TUTORIAL_HPP
#define TEST_TUTORIAL_TEST_TUTORIAL_HPP


#include "libdnf/utils/temp.hpp"

#include <cppunit/extensions/HelperMacros.h>


class TutorialTest : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE(TutorialTest);

    CPPUNIT_TEST(test_create_base);
    CPPUNIT_TEST(test_load_repo);

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() override;
    void tearDown() override;

    void test_create_base();
    void test_load_repo();

private:
    std::string baseurl = PROJECT_BINARY_DIR "/test/data/repos-rpm/rpm-repo1/";

    std::unique_ptr<libdnf::utils::TempDir> temp;
    std::string installroot;
};


#endif  // TEST_TUTORIAL_TEST_TUTORIAL_HPP
