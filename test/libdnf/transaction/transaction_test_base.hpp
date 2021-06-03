// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_TEST_TRANSACTION_TRANSACTION_TEST_BASE_HPP
#define LIBDNF_TEST_TRANSACTION_TRANSACTION_TEST_BASE_HPP


#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include "libdnf/base/base.hpp"
#include "libdnf/utils/temp.hpp"

#include <memory>


class TransactionTestBase : public CppUnit::TestCase {
public:
    void setUp() override;
    void tearDown() override;

protected:
    std::unique_ptr<libdnf::Base> new_base();
    std::unique_ptr<libdnf::utils::TempDir> persistdir;
};


#endif  // LIBDNF_TEST_TRANSACTION_TRANSACTION_TEST_BASE_HPP
