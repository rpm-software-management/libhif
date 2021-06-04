// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "test_utf8.hpp"

#include "libdnf-cli/utils/utf8.hpp"

#include <clocale>


CPPUNIT_TEST_SUITE_REGISTRATION(UTF8Test);


void UTF8Test::setUp() {
    // wide characters do not work at all until we set locales in the code
    setlocale(LC_ALL, "C.UTF-8");

    hello_world_en = "Hello world!";
    hello_world_cs = "Ahoj světe!";
    hello_world_cn = "你好，世界！";
    hello_world_ja = "こんにちは世界！";
}


void UTF8Test::tearDown() {}


void UTF8Test::test_length_en() {
    auto size = hello_world_en.size();
    CPPUNIT_ASSERT_EQUAL(12ul, size);

    auto length = libdnf::cli::utils::utf8::length(hello_world_en);
    CPPUNIT_ASSERT_EQUAL(12ul, length);
}


void UTF8Test::test_length_cs() {
    auto size = hello_world_cs.size();
    CPPUNIT_ASSERT_EQUAL(12ul, size);

    auto length = libdnf::cli::utils::utf8::length(hello_world_cs);
    CPPUNIT_ASSERT_EQUAL(11ul, length);
}


void UTF8Test::test_length_cn() {
    auto size = hello_world_cn.size();
    CPPUNIT_ASSERT_EQUAL(18ul, size);

    auto length = libdnf::cli::utils::utf8::length(hello_world_cn);
    CPPUNIT_ASSERT_EQUAL(6ul, length);
}


void UTF8Test::test_length_ja() {
    auto size = hello_world_ja.size();
    CPPUNIT_ASSERT_EQUAL(24ul, size);

    auto length = libdnf::cli::utils::utf8::length(hello_world_ja);
    CPPUNIT_ASSERT_EQUAL(8ul, length);
}


void UTF8Test::test_width_en() {
    auto size = hello_world_en.size();
    CPPUNIT_ASSERT_EQUAL(12ul, size);

    auto width = libdnf::cli::utils::utf8::width(hello_world_en);
    CPPUNIT_ASSERT_EQUAL(12ul, width);
}


void UTF8Test::test_width_cs() {
    auto size = hello_world_cs.size();
    CPPUNIT_ASSERT_EQUAL(12ul, size);

    auto width = libdnf::cli::utils::utf8::width(hello_world_cs);
    CPPUNIT_ASSERT_EQUAL(11ul, width);
}


void UTF8Test::test_width_cn() {
    auto size = hello_world_cn.size();
    CPPUNIT_ASSERT_EQUAL(18ul, size);

    auto width = libdnf::cli::utils::utf8::width(hello_world_cn);
    CPPUNIT_ASSERT_EQUAL(12ul, width);
}


void UTF8Test::test_width_ja() {
    auto size = hello_world_ja.size();
    CPPUNIT_ASSERT_EQUAL(24ul, size);

    auto width = libdnf::cli::utils::utf8::width(hello_world_ja);
    CPPUNIT_ASSERT_EQUAL(16ul, width);
}


void UTF8Test::test_substr_length_en() {
    // the whole string
    CPPUNIT_ASSERT(hello_world_en == libdnf::cli::utils::utf8::substr_length(hello_world_en));

    // substring, length=5
    CPPUNIT_ASSERT("world" == libdnf::cli::utils::utf8::substr_length(hello_world_en, 6, 5));

    // substring, length=6
    CPPUNIT_ASSERT("world!" == libdnf::cli::utils::utf8::substr_length(hello_world_en, 6, 6));
}


void UTF8Test::test_substr_length_cs() {
    // the whole string
    CPPUNIT_ASSERT(hello_world_cs == libdnf::cli::utils::utf8::substr_length(hello_world_cs));

    // substring, length=4
    CPPUNIT_ASSERT("svět" == libdnf::cli::utils::utf8::substr_length(hello_world_cs, 5, 4));

    // substring, length=5
    CPPUNIT_ASSERT("světe" == libdnf::cli::utils::utf8::substr_length(hello_world_cs, 5, 5));

    // skip after the multi-byte character
    CPPUNIT_ASSERT("te!" == libdnf::cli::utils::utf8::substr_length(hello_world_cs, 8, 3));
}


void UTF8Test::test_substr_length_cn() {
    // the whole string
    CPPUNIT_ASSERT(hello_world_cn == libdnf::cli::utils::utf8::substr_length(hello_world_cn));

    // substring, length=3
    CPPUNIT_ASSERT("，世界" == libdnf::cli::utils::utf8::substr_length(hello_world_cn, 2, 3));

    // substring, length=4
    CPPUNIT_ASSERT("，世界！" == libdnf::cli::utils::utf8::substr_length(hello_world_cn, 2, 4));
}


void UTF8Test::test_substr_length_ja() {
    // the whole string
    CPPUNIT_ASSERT(hello_world_ja == libdnf::cli::utils::utf8::substr_length(hello_world_ja));

    // substring, length=4
    CPPUNIT_ASSERT("にちは世" == libdnf::cli::utils::utf8::substr_length(hello_world_ja, 2, 4));

    // substring, length=5
    CPPUNIT_ASSERT("にちは世界" == libdnf::cli::utils::utf8::substr_length(hello_world_ja, 2, 5));
}


void UTF8Test::test_substr_width_en() {
    // the whole string
    CPPUNIT_ASSERT(hello_world_en == libdnf::cli::utils::utf8::substr_width(hello_world_en));

    // substring, width=5
    CPPUNIT_ASSERT("world" == libdnf::cli::utils::utf8::substr_width(hello_world_en, 6, 5));

    // substring, width=6
    CPPUNIT_ASSERT("world!" == libdnf::cli::utils::utf8::substr_width(hello_world_en, 6, 6));
}


void UTF8Test::test_substr_width_cs() {
    // the whole string
    CPPUNIT_ASSERT(hello_world_cs == libdnf::cli::utils::utf8::substr_width(hello_world_cs));

    // substring, width=4
    CPPUNIT_ASSERT("svět" == libdnf::cli::utils::utf8::substr_width(hello_world_cs, 5, 4));

    // substring, width=5
    CPPUNIT_ASSERT("světe" == libdnf::cli::utils::utf8::substr_width(hello_world_cs, 5, 5));

    // skip after the multi-byte character
    CPPUNIT_ASSERT("te!" == libdnf::cli::utils::utf8::substr_width(hello_world_cs, 8, 3));
}


void UTF8Test::test_substr_width_cn() {
    // the whole string
    CPPUNIT_ASSERT(hello_world_cn == libdnf::cli::utils::utf8::substr_width(hello_world_cn));

    // substring, width=4
    CPPUNIT_ASSERT("，世" == libdnf::cli::utils::utf8::substr_width(hello_world_cn, 2, 4));

    // substring, width=5
    // the additional character has width 2 -> it doesn't fit the width and the result is identical to the previous one
    CPPUNIT_ASSERT("，世" == libdnf::cli::utils::utf8::substr_width(hello_world_cn, 2, 5));
}


void UTF8Test::test_substr_width_ja() {
    // the whole string
    CPPUNIT_ASSERT(hello_world_ja == libdnf::cli::utils::utf8::substr_width(hello_world_ja));

    // substring, width=4
    CPPUNIT_ASSERT("にち" == libdnf::cli::utils::utf8::substr_width(hello_world_ja, 2, 4));

    // substring, width=5
    // the additional character has width 2 -> it doesn't fit the width and the result is identical to the previous one
    CPPUNIT_ASSERT("にち" == libdnf::cli::utils::utf8::substr_width(hello_world_ja, 2, 5));
}


//substr_width
