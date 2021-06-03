# SPDX-License-Identifier: GPL-2.0-or-later
# SPDX-FileCopyrightText: Copyright contributors to the libdnf project


use strict;
use warnings;

use Test::More;
use Test::Exception;

use libdnf::base;

{
    # Creates a new Base object
    my $base = new libdnf::base::Base();
    # Tests returned value
    ok(defined $base, 'new libdnf::base::Base() returned something');
    ok($base->isa('libdnf::base::Base'), "  and it's the right class" );

    # Attempts to call some methods from Base object
    my $loger = $base->get_logger();
    my $config = $base->get_config();
    my $repo_sack = $base->get_rpm_repo_sack();
    my $package_sack = $base->get_rpm_package_sack();
}

{
    # Tests WeakPtr returned from a Base object.

    # Creates a new Base object
    my $base = new libdnf::base::Base();

    # Gets a WeakPtr pointing to Vars in the Base object
    my $vars = $base->get_vars();

    # Creates a copy of WeakPtr
    my $vars2 = $vars;

    # Base is valid -> WeakPtr is valid. Sets "test_variable" using WeakPtr vars.
    $vars->set('test_variable', 'value1');

    # Base is valid -> WeakPtr is valid. Gets value of "test_variable" using copy of WeakPtr vars2.
    ok($vars2->get_value('test_variable') eq 'value1');

    # Invalidates Base object
    $base = 0;

    # Base object is invalid. -> Both WeakPtr are invalid. The code must throw an exception.
    throws_ok( sub { $vars->get_value('test_variable') }, '/InvalidPtr/', 'detected');
    throws_ok( sub { $vars2->get_value('test_variable') }, '/InvalidPtr/', 'detected');
}

done_testing()
