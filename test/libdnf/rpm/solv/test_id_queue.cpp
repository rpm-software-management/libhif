// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "test_id_queue.hpp"

#include "../../../../libdnf/rpm/solv/id_queue.hpp"

#include <iterator>

CPPUNIT_TEST_SUITE_REGISTRATION(IdQueueTest);


void IdQueueTest::setUp() {}


void IdQueueTest::tearDown() {}


// Test push_back(), operator[], size(), and clear()
void IdQueueTest::test_push_back() {
    libdnf::rpm::solv::IdQueue id_queue;
    CPPUNIT_ASSERT(id_queue.size() == 0);

    id_queue.push_back(4);
    CPPUNIT_ASSERT(id_queue.size() == 1);

    id_queue.push_back(3, 2);
    CPPUNIT_ASSERT(id_queue.size() == 3);

    // insert same valule like it is in queue will result in increase of elements
    id_queue.push_back(3);
    CPPUNIT_ASSERT(id_queue.size() == 4);

    // check the order of inserted values
    CPPUNIT_ASSERT(id_queue[0] == 4);
    CPPUNIT_ASSERT(id_queue[1] == 3);
    CPPUNIT_ASSERT(id_queue[2] == 2);
    CPPUNIT_ASSERT(id_queue[3] == 3);
    id_queue.clear();
    CPPUNIT_ASSERT(id_queue.size() == 0);
}

/// Test operators, copy and move constructors
void IdQueueTest::test_operators() {
    libdnf::rpm::solv::IdQueue id_queue_same1;
    libdnf::rpm::solv::IdQueue id_queue_same2;
    libdnf::rpm::solv::IdQueue id_queue_different3;
    libdnf::rpm::solv::IdQueue id_queue_different4;
    id_queue_same1.push_back(3, 2);
    id_queue_same2.push_back(3, 2);
    id_queue_different3.push_back(2, 5);
    id_queue_different4.push_back(2);

    CPPUNIT_ASSERT(id_queue_same1 == id_queue_same2);
    CPPUNIT_ASSERT(id_queue_same1 == id_queue_same1);
    CPPUNIT_ASSERT(id_queue_same1 != id_queue_different3);
    CPPUNIT_ASSERT(id_queue_different4 != id_queue_different3);
    CPPUNIT_ASSERT(id_queue_different3 != id_queue_different4);

    // test copy costructor
    auto copy = id_queue_same1;
    CPPUNIT_ASSERT(id_queue_same1 == copy);
    CPPUNIT_ASSERT(id_queue_same1.size() == 2);

    // test move costructor
    auto move = std::move(id_queue_same1);
    CPPUNIT_ASSERT(id_queue_same1 != move);
    CPPUNIT_ASSERT(id_queue_same1.size() == 0);
    CPPUNIT_ASSERT(move.size() == 2);

    /// test += operator
    CPPUNIT_ASSERT(id_queue_same2.size() == 2);
    CPPUNIT_ASSERT(id_queue_different3.size() == 2);
    id_queue_same2 += id_queue_different3;
    CPPUNIT_ASSERT(id_queue_same2.size() == 4);
    CPPUNIT_ASSERT(id_queue_different3.size() == 2);
    CPPUNIT_ASSERT(id_queue_same2[0] == 3);
    CPPUNIT_ASSERT(id_queue_same2[1] == 2);
    CPPUNIT_ASSERT(id_queue_same2[2] == 2);
    CPPUNIT_ASSERT(id_queue_same2[3] == 5);
}

void IdQueueTest::test_iterator_empty() {
    std::vector<Id> expected = {};
    std::vector<Id> result;
    libdnf::rpm::solv::IdQueue queue;

    for (auto it = queue.begin(); it != queue.end(); it++) {
        result.push_back(*it);
    }
    CPPUNIT_ASSERT(result == expected);
}

void IdQueueTest::test_iterator_full() {
    std::vector<Id> expected = {5, 6, 8, 10, 14};
    libdnf::rpm::solv::IdQueue queue;
    queue.push_back(5, 6);
    queue.push_back(8, 10);
    queue.push_back(14);

    // test begin
    auto it1 = queue.begin();
    CPPUNIT_ASSERT_EQUAL(*it1, 5);

    // test pre-increment operator
    auto it2 = ++it1;
    CPPUNIT_ASSERT_EQUAL(*it1, 6);
    CPPUNIT_ASSERT_EQUAL(*it2, 6);

    // test post-increment operator
    auto it3 = it2++;
    CPPUNIT_ASSERT_EQUAL(*it2, 8);
    CPPUNIT_ASSERT_EQUAL(*it3, 6);

    // test move back to the begin
    it2.begin();
    CPPUNIT_ASSERT_EQUAL(*it2, 5);
    CPPUNIT_ASSERT(it2 == queue.begin());

    // test increment after begin
    ++it2;
    CPPUNIT_ASSERT_EQUAL(*it2, 6);

    auto it4 = std::next(it2);
    CPPUNIT_ASSERT_EQUAL(*it2, 6);
    CPPUNIT_ASSERT_EQUAL(*it4, 8);

    std::advance(it4, 2);
    CPPUNIT_ASSERT_EQUAL(*it4, 14);

    // test move to the end
    it2.end();
    CPPUNIT_ASSERT(it2 == queue.end());

    // test loop with pre-increment operator
    {
        std::vector<Id> result;
        for (auto it = queue.begin(), end = queue.end(); it != end; ++it) {
            result.push_back(*it);
        }
        CPPUNIT_ASSERT(result == expected);
    }

    // test loop with post-increment operator
    {
        std::vector<Id> result;
        for (auto it = queue.begin(), end = queue.end(); it != end; it++) {
            result.push_back(*it);
        }
        CPPUNIT_ASSERT(result == expected);
    }
}

void IdQueueTest::test_iterator_performance() {
    constexpr int max = 1000000;
    libdnf::rpm::solv::IdQueue queue;
    for (int i = 0; i < max; i++) {
        queue.push_back(i);
    }

    for (int i = 0; i < 10; i++) {
        std::vector<Id> result;
        for (auto it = queue.begin(); it != queue.end(); it++) {
            result.push_back(*it);
        }
    }
}
