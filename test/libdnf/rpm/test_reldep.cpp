// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "test_reldep.hpp"

#include "libdnf/rpm/reldep.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(ReldepTest);


void ReldepTest::setUp() {
    base = std::make_unique<libdnf::Base>();
    sack = base->get_rpm_package_sack();
}


void ReldepTest::test_short_reldep() {
    libdnf::rpm::Reldep a(sack, "labirinto.txt");
    CPPUNIT_ASSERT(std::string(a.get_name()) == "labirinto.txt");
    CPPUNIT_ASSERT(std::string(a.get_relation()) == "");
    CPPUNIT_ASSERT(std::string(a.get_version()) == "");
    CPPUNIT_ASSERT(a.to_string() == "labirinto.txt");

    libdnf::rpm::Reldep b(sack, "labirinto.txt");
    CPPUNIT_ASSERT(a == b);
    CPPUNIT_ASSERT(a.get_id() == b.get_id());

    libdnf::rpm::Reldep c(sack, "vagare");
    CPPUNIT_ASSERT(a != c);
}


void ReldepTest::test_full_reldep() {
    libdnf::rpm::Reldep a(sack, "python3-labirinto = 4.2.0");
    CPPUNIT_ASSERT(std::string(a.get_name()) == "python3-labirinto");
    CPPUNIT_ASSERT(std::string(a.get_relation()) == " = ");
    CPPUNIT_ASSERT(std::string(a.get_version()) == "4.2.0");
    CPPUNIT_ASSERT(a.to_string() == "python3-labirinto = 4.2.0");

    libdnf::rpm::Reldep b(sack, "python3-labirinto > 1.2.0");
    CPPUNIT_ASSERT(a != b);
}


void ReldepTest::test_rich_reldep() {
    libdnf::rpm::Reldep a(sack, "(lab-list if labirinto.txt)");
    CPPUNIT_ASSERT(std::string(a.get_name()) == "lab-list");
    CPPUNIT_ASSERT(std::string(a.get_relation()) == " if ");
    CPPUNIT_ASSERT(std::string(a.get_version()) == "labirinto.txt");
    CPPUNIT_ASSERT(a.to_string() == "(lab-list if labirinto.txt)");

    libdnf::rpm::Reldep b(sack, "(labirinto unless labirinto_c)");
    CPPUNIT_ASSERT(std::string(b.get_name()) == "labirinto");
    CPPUNIT_ASSERT(std::string(b.get_relation()) == " unless ");
    CPPUNIT_ASSERT(std::string(b.get_version()) == "labirinto_c");
    CPPUNIT_ASSERT(b.to_string() == "(labirinto unless labirinto_c)");

    CPPUNIT_ASSERT(a.get_id() != b.get_id());
    CPPUNIT_ASSERT(a != b);
}


void ReldepTest::test_invalid_reldep() {
    CPPUNIT_ASSERT_THROW(libdnf::rpm::Reldep a(sack, "(lab-list if labirinto.txt"), std::runtime_error);
    CPPUNIT_ASSERT_THROW(libdnf::rpm::Reldep a(sack, "labirinto = "), std::runtime_error);
}
