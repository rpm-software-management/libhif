// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "test_set.hpp"

#include "libdnf/common/set.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(SetTest);


void SetTest::setUp() {}


void SetTest::tearDown() {}

// Test constructors, empty(), size(), contains(), clear(), add(), and remove()
void SetTest::test_set_basics() {
    // test default constructor
    libdnf::Set<int> s1;
    CPPUNIT_ASSERT(s1.empty());
    CPPUNIT_ASSERT_EQUAL(s1.size(), static_cast<size_t>(0));

    // test initializer list constructor
    libdnf::Set<int> s2{1, 5};
    CPPUNIT_ASSERT(!s2.empty());
    CPPUNIT_ASSERT_EQUAL(s2.size(), static_cast<size_t>(2));
    CPPUNIT_ASSERT(s2.contains(1) && s2.contains(5) && !s2.contains(8));

    // test copy constructor
    libdnf::Set<int> s3 = s2;
    CPPUNIT_ASSERT(!s3.empty());
    CPPUNIT_ASSERT_EQUAL(s3.size(), static_cast<size_t>(2));
    CPPUNIT_ASSERT(s3.contains(1) && s3.contains(5) && !s3.contains(8));

    // test move constructor
    libdnf::Set<int> s4 = std::move(s2);
    CPPUNIT_ASSERT(!s4.empty());
    CPPUNIT_ASSERT_EQUAL(s4.size(), static_cast<size_t>(2));
    CPPUNIT_ASSERT(s4.contains(1) && s4.contains(5) && !s4.contains(8));

    // test clear()
    s4.clear();
    CPPUNIT_ASSERT(s1.empty());

    s1 = {1, 4, 6};
    s1.add(2);
    CPPUNIT_ASSERT((s1 == libdnf::Set<int>{1, 2, 4, 6}));

    s1 = {1, 4, 6};
    s1.add(4);
    CPPUNIT_ASSERT((s1 == libdnf::Set<int>{1, 4, 6}));

    s1 = {1, 4, 6};
    s1.remove(4);
    CPPUNIT_ASSERT((s1 == libdnf::Set<int>{1, 6}));
}

// test operator ==()
void SetTest::test_set_equal_operator() {
    CPPUNIT_ASSERT((libdnf::Set<int>{1, 2, 4} == libdnf::Set<int>{1, 2, 4}));
    CPPUNIT_ASSERT((libdnf::Set<int>{1, 2, 4} == libdnf::Set<int>{1, 4, 2}));
    CPPUNIT_ASSERT((libdnf::Set<int>{1, 2, 4} == libdnf::Set<int>{1, 4, 2, 1}));
    CPPUNIT_ASSERT(!(libdnf::Set<int>{1, 2, 4} == libdnf::Set<int>{1, 4, 4}));
}

// test operator =()
void SetTest::test_set_assignment_operator() {
    libdnf::Set<int> s1;
    libdnf::Set<int> s2;

    s1 = {1, 4};
    CPPUNIT_ASSERT((s1 == libdnf::Set<int>{1, 4}));

    s2 = s1;
    CPPUNIT_ASSERT((s2 == libdnf::Set<int>{1, 4}));

    s2 = std::move(s1);
    CPPUNIT_ASSERT((s2 == libdnf::Set<int>{1, 4}));
}

// test unary operators |= &= -= ^=
void SetTest::test_set_unary_operators() {
    libdnf::Set<int> s;

    s = {1, 4};
    s |= {2, 4, 5};
    CPPUNIT_ASSERT((s == libdnf::Set<int>{1, 2, 4, 5}));

    s = {1, 2, 3, 4};
    s &= {2, 4, 6};
    CPPUNIT_ASSERT((s == libdnf::Set<int>{2, 4}));

    s = {1, 4, 6};
    s -= {2, 4, 5};
    CPPUNIT_ASSERT((s == libdnf::Set<int>{1, 6}));

    s = {1, 2, 3, 4};
    s ^= {2, 4, 6};
    CPPUNIT_ASSERT((s == libdnf::Set<int>{1, 3, 6}));
}

// test unary methods update(), intersection(), difference(), symetric_difference(), and swap()
void SetTest::test_set_unary_methods() {
    libdnf::Set<int> s;

    s = {1, 4};
    s.update({2, 4, 5});
    CPPUNIT_ASSERT((s == libdnf::Set<int>{1, 2, 4, 5}));

    s = {1, 2, 3, 4};
    s.intersection({2, 4, 6});
    CPPUNIT_ASSERT((s == libdnf::Set<int>{2, 4}));

    s = {1, 4, 6};
    s.difference({2, 4, 5});
    CPPUNIT_ASSERT((s == libdnf::Set<int>{1, 6}));

    s = {1, 2, 3, 4};
    s.symmetric_difference({2, 4, 6});
    CPPUNIT_ASSERT((s == libdnf::Set<int>{1, 3, 6}));

    libdnf::Set<int> s1{1, 4, 6};
    libdnf::Set<int> s2{2, 5, 8};
    s1.swap(s2);
    CPPUNIT_ASSERT((s1 == libdnf::Set<int>{2, 5, 8}));
    CPPUNIT_ASSERT((s2 == libdnf::Set<int>{1, 4, 6}));
}

// test binary operators | & - ^
void SetTest::test_set_binary_operators() {
    // test binary operators | & - ^
    libdnf::Set<int> s1{1, 4, 6};
    libdnf::Set<int> s2{2, 4};
    CPPUNIT_ASSERT(((s1 | s2) == libdnf::Set<int>{1, 2, 4, 6}));
    CPPUNIT_ASSERT(((s1 & s2) == libdnf::Set<int>{4}));
    CPPUNIT_ASSERT(((s1 - s2) == libdnf::Set<int>{1, 6}));
    CPPUNIT_ASSERT(((s1 ^ s2) == libdnf::Set<int>{1, 2, 6}));
}
