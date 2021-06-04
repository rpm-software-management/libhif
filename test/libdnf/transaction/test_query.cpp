// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "test_query.hpp"

#include "libdnf/common/sack/query_cmp.hpp"
#include "libdnf/transaction/transaction.hpp"

#include <string>


using namespace libdnf::transaction;


CPPUNIT_TEST_SUITE_REGISTRATION(TransactionQueryTest);


void TransactionQueryTest::test_filter_id_eq() {
    auto base = new_base();

    // create a new empty transaction
    auto trans = base->get_transaction_sack()->new_transaction();

    // save the transaction
    trans->start();
    trans->finish(TransactionState::DONE);

    // create a new Base to force reading the transaction from disk
    auto base2 = new_base();

    // get the written transaction
    auto q2 = base2->get_transaction_sack()->new_query();
    q2.filter_id(trans->get_id());
    auto trans2 = q2.get();
}


void TransactionQueryTest::test_filter_id_eq_parallel_queries() {
    auto base = new_base();
    auto transaction_sack = base->get_transaction_sack();

    // create a new empty transaction
    auto trans = transaction_sack->new_transaction();

    // save the transaction
    trans->start();
    trans->finish(TransactionState::DONE);

    // create a new Base to force reading the transaction from disk
    auto base2 = new_base();
    auto transaction_sack2 = base2->get_transaction_sack();

    // the sack is empty
    CPPUNIT_ASSERT_EQUAL(0LU, transaction_sack2->get_data().size());

    // create 2 queries that are empty, none of them loaded any transaction yet
    auto q2 = transaction_sack2->new_query();
    auto q3 = transaction_sack2->new_query();

    q2.filter_id(trans->get_id());
    auto trans2 = q2.get();

    // one item loaded into the sack
    CPPUNIT_ASSERT_EQUAL(1LU, transaction_sack2->get_data().size());

    q3.filter_id(trans->get_id());
    auto trans3 = q3.get();

    // query reused the existing item in the sack
    CPPUNIT_ASSERT_EQUAL(1LU, transaction_sack2->get_data().size());

    // finally compare the transactions
    CPPUNIT_ASSERT(trans2 == trans3);
}
