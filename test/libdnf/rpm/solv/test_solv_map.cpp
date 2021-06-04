// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "test_solv_map.hpp"

#include <cstdint>


CPPUNIT_TEST_SUITE_REGISTRATION(SolvMapTest);


void SolvMapTest::setUp() {
    map1 = new libdnf::rpm::solv::SolvMap(32);
    map1->add(0);
    map1->add(2);
    map1->add(28);
    map1->add(30);

    map2 = new libdnf::rpm::solv::SolvMap(32);
    map2->add(0);
    map2->add(1);
}


void SolvMapTest::tearDown() {
    delete map1;
    delete map2;
}


void SolvMapTest::test_add() {
    // test if the maps created in the constructor
    // have the correct bits set

    CPPUNIT_ASSERT(map1->contains(0) == true);
    CPPUNIT_ASSERT(map1->contains(1) == false);
    CPPUNIT_ASSERT(map1->contains(2) == true);

    CPPUNIT_ASSERT(map2->contains(0) == true);
    CPPUNIT_ASSERT(map2->contains(1) == true);
    CPPUNIT_ASSERT(map2->contains(2) == false);

    // add an item that is in the map already
    map1->add(0);
    CPPUNIT_ASSERT(map1->contains(0) == true);

    // test invalid ranges
    CPPUNIT_ASSERT_THROW(map1->add(-1), std::out_of_range);
    CPPUNIT_ASSERT_THROW(map1->add(33), std::out_of_range);
}


void SolvMapTest::test_contains() {
    CPPUNIT_ASSERT(map1->contains(0) == true);
    CPPUNIT_ASSERT(map1->contains(1) == false);

    CPPUNIT_ASSERT(map2->contains(0) == true);
    CPPUNIT_ASSERT(map2->contains(1) == true);

    // test invalid ranges
    CPPUNIT_ASSERT(map2->contains(-1) == false);
    CPPUNIT_ASSERT(map2->contains(33) == false);
}


void SolvMapTest::test_remove() {
    // remove an item that is in the map
    CPPUNIT_ASSERT(map1->contains(0) == true);
    map1->remove(0);
    CPPUNIT_ASSERT(map1->contains(0) == false);

    // remove an item that is not in the map
    map1->remove(0);
    CPPUNIT_ASSERT(map1->contains(1) == false);

    // test invalid ranges
    CPPUNIT_ASSERT_THROW(map1->remove(-1), std::out_of_range);
    CPPUNIT_ASSERT_THROW(map1->remove(33), std::out_of_range);
}


void SolvMapTest::test_map_allocation_range() {
    // allocate map for 25 bits, but in fact map for 4 bytes (32 bits) is allocated
    libdnf::rpm::solv::SolvMap map(25);

    // setting bit 31 works
    map.add(31);

    // setting bit 32 does not work (we're indexing from 0)
    CPPUNIT_ASSERT_THROW(map1->add(32), std::out_of_range);
}


void SolvMapTest::test_union() {
    *map2 |= *map1;
    CPPUNIT_ASSERT(map2->contains(0) == true);
    CPPUNIT_ASSERT(map2->contains(1) == true);
    CPPUNIT_ASSERT(map2->contains(2) == true);
    CPPUNIT_ASSERT(map2->contains(3) == false);
}


void SolvMapTest::test_intersection() {
    *map2 &= *map1;
    CPPUNIT_ASSERT(map2->contains(0) == true);
    CPPUNIT_ASSERT(map2->contains(1) == false);
    CPPUNIT_ASSERT(map2->contains(2) == false);
    CPPUNIT_ASSERT(map2->contains(3) == false);
}


void SolvMapTest::test_difference() {
    *map2 -= *map1;
    CPPUNIT_ASSERT(map2->contains(0) == false);
    CPPUNIT_ASSERT(map2->contains(1) == true);
    CPPUNIT_ASSERT(map2->contains(2) == false);
    CPPUNIT_ASSERT(map2->contains(3) == false);
}


void SolvMapTest::test_iterator_empty() {
    std::vector<Id> expected = {};
    std::vector<Id> result;
    libdnf::rpm::solv::SolvMap map(16);
    for (auto it = map.begin(); it != map.end(); it++) {
        result.push_back(*it);
    }
    CPPUNIT_ASSERT(result == expected);
}


void SolvMapTest::test_iterator_full() {
    std::vector<Id> expected = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    std::vector<Id> result;
    libdnf::rpm::solv::SolvMap map(16);
    for (int i = 0; i < 16; i++) {
        map.add(i);
    }
    for (auto package_id : map) {
        result.push_back(package_id);
    }
    CPPUNIT_ASSERT(result == expected);
}


void SolvMapTest::test_iterator_sparse() {
    std::vector<Id> expected = {4, 6, 10, 11, 12};

    libdnf::rpm::solv::SolvMap map(16);
    for (auto it : expected) {
        map.add(it);
    }

    // test begin
    auto it1 = map.begin();
    CPPUNIT_ASSERT_EQUAL(*it1, 4);

    // test pre-increment operator
    auto it2 = ++it1;
    CPPUNIT_ASSERT_EQUAL(*it1, 6);
    CPPUNIT_ASSERT_EQUAL(*it2, 6);

    // test post-increment operator
    auto it3 = it2++;
    CPPUNIT_ASSERT_EQUAL(*it2, 10);
    CPPUNIT_ASSERT_EQUAL(*it3, 6);

    // test move back to begin
    it2.begin();
    CPPUNIT_ASSERT_EQUAL(*it2, 4);

    // test increment after begin
    ++it2;
    CPPUNIT_ASSERT_EQUAL(*it2, 6);

    // test end
    it2.end();
    CPPUNIT_ASSERT(it2 == map.end());

    // test loop with pre-increment operator
    {
        std::vector<Id> result;
        for (auto it = map.begin(), end = map.end(); it != end; ++it) {
            result.push_back(*it);
        }
        CPPUNIT_ASSERT(result == expected);
    }

    // test loop with post-increment operator
    {
        std::vector<Id> result;
        for (auto it = map.begin(), end = map.end(); it != end; it++) {
            result.push_back(*it);
        }
        CPPUNIT_ASSERT(result == expected);
    }

    // test jump to existing package
    it1.jump(11);
    CPPUNIT_ASSERT_EQUAL(*it1, 11);

    // test jump behind last existing package
    it1.jump(14);
    CPPUNIT_ASSERT(it1 == map.end());

    // test jump to existing package
    it1.jump(6);
    CPPUNIT_ASSERT_EQUAL(*it1, 6);

    // test jump to non-existing package, moves to the next existing
    it1.jump(7);
    CPPUNIT_ASSERT_EQUAL(*it1, 10);

    // test jump behind map
    it1.jump(240);
    CPPUNIT_ASSERT(it1 == map.end());

    // test jump to negative id, sets to begin
    it1.jump(-240);
    CPPUNIT_ASSERT_EQUAL(*it1, 4);

    // test increment after jump to negative id
    ++it1;
    CPPUNIT_ASSERT_EQUAL(*it1, 6);
}


void SolvMapTest::test_iterator_performance_empty() {
    // initialize a map filed with zeros
    constexpr int max = 1000000;
    libdnf::rpm::solv::SolvMap map(max);

    for (int i = 0; i < 500; i++) {
        std::vector<Id> result;
        for (auto it = map.begin(); it != map.end(); it++) {
            result.push_back(*it);
        }
    }
}


void SolvMapTest::test_iterator_performance_full() {
    // initialize a map filed with ones
    constexpr int max = 1000000;
    libdnf::rpm::solv::SolvMap map(max);
    memset(map.get_map()->map, 255, static_cast<std::size_t>(map.get_map()->size));

    for (int i = 0; i < 500; i++) {
        std::vector<Id> result;
        for (auto it = map.begin(); it != map.end(); it++) {
            result.push_back(*it);
        }
    }
}


void SolvMapTest::test_iterator_performance_4bits() {
    // initialize a map filed with 00001111 bytes
    constexpr int max = 1000000;
    libdnf::rpm::solv::SolvMap map(max);
    memset(map.get_map()->map, 15, static_cast<std::size_t>(map.get_map()->size));

    for (int i = 0; i < 500; i++) {
        std::vector<Id> result;
        for (auto it = map.begin(); it != map.end(); it++) {
            result.push_back(*it);
        }
    }
}
