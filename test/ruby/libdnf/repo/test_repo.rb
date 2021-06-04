# SPDX-License-Identifier: GPL-2.0-or-later
# SPDX-FileCopyrightText: Copyright contributors to the libdnf project


require 'test/unit'
require 'tmpdir'
include Test::Unit::Assertions

require 'libdnf/base'

class TestRepo < Test::Unit::TestCase
    def test_repo()
        base = Base::Base.new()

        # Sets path to cache directory.
        tmpdir = Dir.mktmpdir("libdnf-ruby-")
        base.get_config().cachedir().set(Conf::Option::Priority_RUNTIME, tmpdir)

        repo_sack = Repo::RepoSack.new(base)
        package_sack = Rpm::PackageSack.new(base)

        # Creates system repository and loads it into rpm::PackageSack.
        package_sack.create_system_repo(false)

        # Creates new repositories in the repo_sack
        repo = repo_sack.new_repo("repomd-repo1")

        # Tunes repository configuration (baseurl is mandatory)
        repo_path = File.join(Dir.getwd(), '../../../test/data/repos-repomd/repomd-repo1/')
        baseurl = 'file://' + repo_path
        repo_cfg = repo.get_config()
        repo_cfg.baseurl().set(Conf::Option::Priority_RUNTIME, baseurl)

        # Loads repository into rpm::Repo.
        repo.load()

        # Loads rpm::Repo into rpm::PackageSack
        package_sack.load_repo(repo.get(), Rpm::PackageSack::LoadRepoFlags_USE_PRESTO |
                            Rpm::PackageSack::LoadRepoFlags_USE_UPDATEINFO | Rpm::PackageSack::LoadRepoFlags_USE_OTHER)

        # Remove the cache directory.
        FileUtils.remove_entry(tmpdir)
    end
end
