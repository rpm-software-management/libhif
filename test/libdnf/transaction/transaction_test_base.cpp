// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "transaction_test_base.hpp"


void TransactionTestBase::setUp() {
    persistdir = std::make_unique<libdnf::utils::TempDir>("libdnf_unittest_");
}


void TransactionTestBase::tearDown() {
}


std::unique_ptr<libdnf::Base> TransactionTestBase::new_base() {
    auto new_base = std::make_unique<libdnf::Base>();
    new_base->get_config().persistdir().set(libdnf::Option::Priority::RUNTIME, persistdir->get_path());
    return new_base;
}
