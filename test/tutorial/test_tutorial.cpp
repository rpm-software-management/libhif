// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "test_tutorial.hpp"

#include "libdnf/base/base.hpp"


CPPUNIT_TEST_SUITE_REGISTRATION(TutorialTest);


void TutorialTest::setUp() {
    temp = std::make_unique<libdnf::utils::TempDir>("libdnf_unittest_");
    installroot = temp->get_path().native() + "/installroot";
}


void TutorialTest::tearDown() {
}


void TutorialTest::test_create_base() {
    #include "session/create_base.cpp"
}


void TutorialTest::test_load_repo() {
    #include "session/create_base.cpp"
    #include "repo/load_repo.cpp"
}
