# SPDX-License-Identifier: GPL-2.0-or-later
# SPDX-FileCopyrightText: Copyright contributors to the libdnf project


import unittest
import tempfile
import shutil
import os

import libdnf.base
import libdnf.logger


class TestRepo(unittest.TestCase):
    def test_repo(self):
        base = libdnf.base.Base()

        # Sets path to cache directory.
        tmpdir = tempfile.mkdtemp(prefix="libdnf-python3-")
        base.get_config().cachedir().set(libdnf.conf.Option.Priority_RUNTIME, tmpdir)

        repo_sack = libdnf.repo.RepoSack(base)
        package_sack = libdnf.rpm.PackageSack(base)

        # Creates system repository and loads it into rpm::PackageSack.
        package_sack.create_system_repo(False)

        # Creates new repositories in the repo_sack
        repo = repo_sack.new_repo("repomd-repo1")

        # Tunes repository configuration (baseurl is mandatory)
        repo_path = os.path.join(os.getcwd(), "../../../test/data/repos-repomd/repomd-repo1/")
        baseurl = "file://" + repo_path
        repo_cfg = repo.get_config()
        repo_cfg.baseurl().set(libdnf.conf.Option.Priority_RUNTIME, baseurl)

        # Loads repository into rpm::Repo.
        repo.load()

        # Loads rpm::Repo into rpm::PackageSack
        PackageSack = libdnf.rpm.PackageSack
        package_sack.load_repo(repo.get(), PackageSack.LoadRepoFlags_USE_PRESTO | PackageSack.LoadRepoFlags_USE_UPDATEINFO |
                            PackageSack.LoadRepoFlags_USE_OTHER)

        # Remove the cache directory.
        shutil.rmtree(tmpdir)
