# SPDX-License-Identifier: GPL-2.0-or-later
# SPDX-FileCopyrightText: Copyright contributors to the libdnf project


require 'test/unit'
include Test::Unit::Assertions

require 'libdnf/base'

class TestBase < Test::Unit::TestCase
    def test_base()
        base = Base::Base.new()
        loger = base.get_logger()
        config = base.get_config()
        repo_sack = base.get_rpm_repo_sack()
        package_sack = base.get_rpm_package_sack()
    end

    def test_weak_ptr()
        # Creates a new Base object
        base = Base::Base.new()

        # Gets a WeakPtr pointing to Vars in the Base object
        vars = base.get_vars()

        # Creates a copy of WeakPtr
        vars2 = vars

        # Base is valid -> WeakPtr is valid. Sets "test_variable" using WeakPtr vars.
        vars.set("test_variable", "value1")

        # Base is valid -> WeakPtr is valid. Gets value of "test_variable" using copy of WeakPtr vars2.
        assert_equal("value1", vars2.get_value("test_variable"))

        # Invalidates Base object
        base = nil
        # Ensure gargabe collection
        GC.start

        # Base object is invalid. -> Both WeakPtr are invalid. The code must throw an exception.
        assert_raise do
            vars.get_value("test_variable")
        end
        assert_raise do
            vars2.get_value("test_variable")
        end
    end
end
