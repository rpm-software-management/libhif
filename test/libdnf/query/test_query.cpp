// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "test_query.hpp"

#include "libdnf/common/sack/query.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(QueryTest);

class QueryItem {
public:
    bool enabled;
    int id;
    std::string name;

    bool operator==(const QueryItem & other) const noexcept {
        return enabled == other.enabled && id == other.id && name == other.name;
    }
    bool operator<(const QueryItem & other) const noexcept { return id < other.id; }
};

// Simple query implementation based on libdnf::sack::Query that works on the QueryItem instances and implements 3 filters.
class TestQuery : public libdnf::sack::Query<QueryItem> {
public:
    using Query<QueryItem>::Query;

    TestQuery & filter_enabled(bool enabled);
    TestQuery & filter_id(int64_t id, libdnf::sack::QueryCmp cmp);
    TestQuery & filter_name(const std::string & name, libdnf::sack::QueryCmp cmp);

private:
    struct F {
        static bool enabled(const QueryItem & obj) { return obj.enabled; }
        static int64_t id(const QueryItem & obj) { return obj.id; }
        static std::string name(const QueryItem & obj) { return obj.name; }
    };
};

TestQuery & TestQuery::filter_enabled(bool enabled) {
    filter(F::enabled, enabled, libdnf::sack::QueryCmp::EQ);
    return *this;
}

TestQuery & TestQuery::filter_id(int64_t id, libdnf::sack::QueryCmp cmp) {
    filter(F::id, id, cmp);
    return *this;
}

TestQuery & TestQuery::filter_name(const std::string & name, libdnf::sack::QueryCmp cmp) {
    filter(F::name, name, cmp);
    return *this;
}


void QueryTest::setUp() {}


void QueryTest::tearDown() {}


void QueryTest::test_query_basics() {
    TestQuery q(libdnf::Set<QueryItem>{{true, 4, "item4"}, {false, 6, "item6"}, {true, 10, "item10"}});
    CPPUNIT_ASSERT_EQUAL(q.size(), static_cast<size_t>(3));

    auto q1 = q;
    CPPUNIT_ASSERT_EQUAL(q1.size(), static_cast<size_t>(3));

    // test filter_enabled()
    q1.filter_enabled(true);
    CPPUNIT_ASSERT_EQUAL(q1.size(), static_cast<size_t>(2));
    CPPUNIT_ASSERT((q1 == libdnf::Set<QueryItem>{{true, 4, "item4"}, {true, 10, "item10"}}));

    // test filter_name()
    q1.filter_name("item10", libdnf::sack::QueryCmp::EQ);
    CPPUNIT_ASSERT_EQUAL(q1.size(), static_cast<size_t>(1));
    CPPUNIT_ASSERT((q1 == libdnf::Set<QueryItem>{{true, 10, "item10"}}));

    // test filter_id()
    q1 = q;
    q1.filter_id(6, libdnf::sack::QueryCmp::GTE);
    CPPUNIT_ASSERT_EQUAL(q1.size(), static_cast<size_t>(2));
    CPPUNIT_ASSERT((q1 == libdnf::Set<QueryItem>{{false, 6, "item6"}, {true, 10, "item10"}}));

    // test chaining of filter calling
    q1 = q;
    q1.filter_name("item?", libdnf::sack::QueryCmp::GLOB).filter_enabled(false);
    CPPUNIT_ASSERT_EQUAL(q1.size(), static_cast<size_t>(1));
    CPPUNIT_ASSERT((q1 == libdnf::Set<QueryItem>{{false, 6, "item6"}}));
}
