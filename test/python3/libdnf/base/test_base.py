# SPDX-License-Identifier: GPL-2.0-or-later
# SPDX-FileCopyrightText: Copyright contributors to the libdnf project


import unittest

import libdnf.base


class TestBase(unittest.TestCase):
    def test_base(self):
        base = libdnf.base.Base()
        loger = base.get_logger()
        config = base.get_config()
        repo_sack = base.get_rpm_repo_sack()
        package_sack = base.get_rpm_package_sack()

    def test_weak_ptr(self):
        # Creates a new Base object
        base = libdnf.base.Base()

        # Gets a WeakPtr pointing to Vars in the Base object
        vars = base.get_vars()

        # Creates a copy of WeakPtr
        vars2 = vars

        # Base is valid -> WeakPtr is valid. Sets "test_variable" using WeakPtr vars.
        vars.set("test_variable", "value1")

        # Base is valid -> WeakPtr is valid. Gets value of "test_variable" using copy of WeakPtr vars2.
        self.assertEqual(vars2.get_value("test_variable"), "value1")

        # Invalidates Base object
        base = None

        # Base object is invalid. -> Both WeakPtr are invalid. The code must throw an exception.
        with self.assertRaisesRegex(RuntimeError, '.*InvalidPtr.*'):
            vars.get_value("test_variable")
        with self.assertRaisesRegex(RuntimeError, '.*InvalidPtr.*'):
            vars2.get_value("test_variable")
