// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "test_base.hpp"

#include "libdnf/base/base.hpp"


CPPUNIT_TEST_SUITE_REGISTRATION(BaseTest);

void BaseTest::test_weak_ptr() {
    // Creates a new Base object
    auto base = std::make_unique<libdnf::Base>();

    // Gets a WeakPtr pointing to Vars in the Base object
    auto vars = base->get_vars();

    // Creates a copy of WeakPtr
    auto vars2 = vars;

    // Base is valid -> WeakPtr is valid. Sets "test_variable" using WeakPtr vars.
    vars->set("test_variable", "value1");

    // Base is valid -> WeakPtr is valid. Gets value of "test_variable" using copy of WeakPtr vars2.
    CPPUNIT_ASSERT_EQUAL(std::string("value1"), vars2->get_value("test_variable"));

    // Invalidates Base object
    base.reset();

    // Base object is invalid. -> Both WeakPtr are invalid. The code must throw an exception.
    CPPUNIT_ASSERT_THROW(vars->get_value("test_variable"), libdnf::VarsWeakPtr::InvalidPtr);
    CPPUNIT_ASSERT_THROW(vars2->get_value("test_variable"), libdnf::VarsWeakPtr::InvalidPtr);
}
