# SPDX-License-Identifier: GPL-2.0-or-later
# SPDX-FileCopyrightText: Copyright contributors to the libdnf project


import unittest
import tempfile
import shutil
import os

import libdnf


class TestPackageQuery(unittest.TestCase):
    def setUp(self):
        self.base = libdnf.base.Base()

        # Sets path to cache directory.
        self.tmpdir = tempfile.mkdtemp(prefix="libdnf-python3-")
        self.base.get_config().cachedir().set(libdnf.conf.Option.Priority_RUNTIME, self.tmpdir)

        self.repo_sack = libdnf.repo.RepoSack(self.base)
        self.sack = self.base.get_rpm_package_sack()

        # Creates new repositories in the repo_sack
        repo = self.repo_sack.new_repo("repomd-repo1")

        # Tunes repository configuration (baseurl is mandatory)
        repo_path = os.path.join(os.getcwd(), "../../../test/data/repos-repomd/repomd-repo1/")
        baseurl = "file://" + repo_path
        repo_cfg = repo.get_config()
        repo_cfg.baseurl().set(libdnf.conf.Option.Priority_RUNTIME, baseurl)

        # Loads repository into rpm::Repo.
        repo.load()

        # Loads rpm::Repo into rpm::PackageSack
        self.sack.load_repo(repo.get(), libdnf.rpm.PackageSack.LoadRepoFlags_NONE)

    def tearDown(self):
        # Remove the cache directory.
        shutil.rmtree(self.tmpdir)

    def test_size(self):
        query = libdnf.rpm.PackageQuery(self.sack)
        self.assertEqual(query.size(), 3)

    def test_iterate_package_query(self):
        query = libdnf.rpm.PackageQuery(self.sack)

        # Iterates over reference "query".
        prev_id = 0
        for pkg in query:
            id = pkg.get_id().id
            self.assertGreater(id, prev_id)
            prev_id = id
        self.assertLess(prev_id, self.sack.get_nsolvables())
        self.assertGreaterEqual(prev_id, query.size())

        # Similar to above, but longer notation.
        query_iterator = iter(query)
        prev_id = 0
        while True:
            try:
                pkg = next(query_iterator)
                id = pkg.get_id().id
                self.assertGreater(id, prev_id)
                prev_id = id
            except StopIteration:
                break
        self.assertLess(prev_id, self.sack.get_nsolvables())
        self.assertGreaterEqual(prev_id, query.size())

    def test_iterate_package_query2(self):
        # Tests the iteration of an unreferenced PackageQuery object. The iterator must hold a reference
        # to the iterated object, otherwise the gargabe collector can remove the object.

        # Iterates directly over "libdnf.rpm.PackageQuery(self.sack)" result. No helping reference.
        prev_id = 0
        for pkg in libdnf.rpm.PackageQuery(self.sack):
            id = pkg.get_id().id
            self.assertGreater(id, prev_id)
            prev_id = id
        self.assertLess(prev_id, self.sack.get_nsolvables())
        self.assertGreaterEqual(prev_id, libdnf.rpm.PackageQuery(self.sack).size())

        # Another test. The iterator is created from the "query" reference, but the reference
        # is removed (set to "None") before starting the iteration.
        query = libdnf.rpm.PackageQuery(self.sack)
        query_iterator = iter(query)
        query = None
        prev_id = 0
        while True:
            try:
                pkg = next(query_iterator)
                id = pkg.get_id().id
                self.assertGreater(id, prev_id)
                prev_id = id
            except StopIteration:
                break
        self.assertLess(prev_id, self.sack.get_nsolvables())
        self.assertGreaterEqual(prev_id, libdnf.rpm.PackageQuery(self.sack).size())

    def test_filter_name(self):
        # Test QueryCmp::EQ
        query = libdnf.rpm.PackageQuery(self.sack)
        query.filter_name(["pkg"])
        self.assertEqual(query.size(), 1)
        # TODO(dmach): implement __str__()
        self.assertEqual([i.get_nevra() for i in query], ["pkg-1.2-3.x86_64"])

        # ---

        # Test QueryCmp::GLOB
        query = libdnf.rpm.PackageQuery(self.sack)
        query.filter_name(["pk*"], libdnf.common.QueryCmp_GLOB)
        self.assertEqual(query.size(), 2)
        # TODO(dmach): implement __str__()
        self.assertEqual([i.get_nevra() for i in query], ["pkg-1.2-3.x86_64", "pkg-libs-1:1.3-4.x86_64"])
