# SPDX-License-Identifier: GPL-2.0-or-later
# SPDX-FileCopyrightText: Copyright contributors to the libdnf project


use strict;
use warnings;
no if $] >= 5.010, warnings => qw(experimental::smartmatch);

use Test::More;
use Cwd qw(cwd);
use File::Temp qw(tempdir);
use File::Spec::Functions 'catfile';


use libdnf::base;

my $base = new libdnf::base::Base();

# Sets path to cache directory.
my $tmpdir = tempdir("libdnf-perl5-XXXX", TMPDIR => 1, CLEANUP => 1);
$base->get_config()->cachedir()->set($libdnf::conf::Option::Priority_RUNTIME, $tmpdir);

my $repo_sack = new libdnf::repo::RepoSack($base);
my $sack = $base->get_rpm_package_sack();

# Creates new repositories in the repo_sack
my $repo = $repo_sack->new_repo("repomd-repo1");

# Tunes repository configuration (baseurl is mandatory)
my $repo_path = catfile(cwd, "../../../test/data/repos-repomd/repomd-repo1/");
my $baseurl = "file://" . $repo_path;
my $repo_cfg = $repo->get_config();
$repo_cfg->baseurl()->set($libdnf::conf::Option::Priority_RUNTIME, $baseurl);

# Loads repository into rpm::Repo.
$repo->load();

# Loads rpm::Repo into rpm::PackageSack
$sack->load_repo($repo->get(), $libdnf::rpm::PackageSack::LoadRepoFlags_NONE);

#test_size()
{
    my $query = new libdnf::rpm::PackageQuery($sack);
    is($query->size(), 3);
}

my @nevras = ("CQRlib-1.1.1-4.fc29.src", "CQRlib-1.1.1-4.fc29.x86_64");
my @nevras_contains = ("CQRlib-1.1.1-4.fc29.src", "CQRlib-1.1.1-4.fc29.x86_64",
                       "CQRlib-devel-1.1.2-16.fc29.src", "CQRlib-devel-1.1.2-16.fc29.x86_64");
my @full_nevras = ("CQRlib-0:1.1.1-4.fc29.src", "CQRlib-0:1.1.1-4.fc29.x86_64",
                   "nodejs-1:5.12.1-1.fc29.src", "nodejs-1:5.12.1-1.fc29.x86_64");

# Test QueryCmp::EQ
{
    my $query = new libdnf::rpm::PackageQuery($sack);
    $query->filter_name(["pkg"]);
    is($query->size(), 1);

    my @expected = ("pkg-1.2-3.x86_64");
    my @actual = ();
    # TODO(dmach): implement iteration over query
    my $it = $query->begin();
    my $e = $query->end();
    while ($it != $e) {
        push @actual, $it->value()->get_nevra();
        $it->next();
    }
    ok(@actual ~~ @expected);
}

# Test QueryCmp::GLOB
{
    my $query = new libdnf::rpm::PackageQuery($sack);
    $query->filter_name(["pk*"], $libdnf::common::QueryCmp_GLOB);
    is($query->size(), 2);

    my @expected = ("pkg-1.2-3.x86_64", "pkg-libs-1:1.3-4.x86_64");
    my @actual = ();
    # TODO(dmach): implement iteration over query
    my $it = $query->begin();
    my $e = $query->end();
    while ($it != $e) {
        #ok(exists($nevras_map{$it->value()->get_nevra()}));
        push @actual, $it->value()->get_nevra();
        $it->next();
    }
    ok(@actual ~~ @expected);
}

done_testing();
