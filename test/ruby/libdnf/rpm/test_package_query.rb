# SPDX-License-Identifier: GPL-2.0-or-later
# SPDX-FileCopyrightText: Copyright contributors to the libdnf project


require 'test/unit'
require 'tmpdir'
include Test::Unit::Assertions

require 'libdnf/base'

class TestSimpleNumber < Test::Unit::TestCase

    def setup()
        @base = Base::Base.new()

        # Sets path to cache directory.
        @tmpdir = Dir.mktmpdir("libdnf-ruby-")
        @base.get_config().cachedir().set(Conf::Option::Priority_RUNTIME, @tmpdir)

        @repo_sack = Repo::RepoSack.new(@base)
        @sack = @base.get_rpm_package_sack()

        # Creates new repositories in the repo_sack
        @repo = @repo_sack.new_repo('repomd-repo1')

        # Tunes repository configuration (baseurl is mandatory)
        repo_path = File.join(Dir.getwd(), '../../../test/data/repos-repomd/repomd-repo1/')
        baseurl = 'file://' + repo_path
        repo_cfg = @repo.get_config()
        repo_cfg.baseurl().set(Conf::Option::Priority_RUNTIME, baseurl)

        # Loads repository into rpm::Repo.
        @repo.load()

        # Loads rpm::Repo into rpm::PackageSack
        @sack.load_repo(@repo.get(), Rpm::PackageSack::LoadRepoFlags_NONE)
    end

    def teardown()
        # Remove the cache directory.
        FileUtils.remove_entry(@tmpdir)
    end

    def test_size()
        query = Rpm::PackageQuery.new(@sack)
        assert_equal(3, query.size(), 'Number of items in the newly created query')
    end

    def test_filter_name()
        # Test QueryCmp::EQ
        query = Rpm::PackageQuery.new(@sack)
        query.filter_name(["pkg"])
        assert_equal(1, query.size())

        # TODO(dmach): implement each() so the query can be easily iterated or converted to an array
        actual = []
        it = query.begin()
        while it != query.end()
            actual += [it.value.get_nevra]
            it.next()
        end

        assert_equal(["pkg-1.2-3.x86_64"], actual)

        # ---

        # Test QueryCmp::GLOB
        query = Rpm::PackageQuery.new(@sack)
        query.filter_name(["pk*"], Common::QueryCmp_GLOB)
        assert_equal(2, query.size())

        # TODO(dmach): implement each() so the query can be easily iterated or converted to an array
        actual = []
        it = query.begin()
        while it != query.end()
            actual += [it.value.get_nevra]
            it.next()
        end

        assert_equal(["pkg-1.2-3.x86_64", "pkg-libs-1:1.3-4.x86_64"], actual)
    end

end
