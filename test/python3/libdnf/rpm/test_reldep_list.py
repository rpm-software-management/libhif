# SPDX-License-Identifier: GPL-2.0-or-later
# SPDX-FileCopyrightText: Copyright contributors to the libdnf project


import unittest
import tempfile
import shutil
import os

import libdnf


class TestReldepList(unittest.TestCase):
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
        clsPackageSack = libdnf.rpm.PackageSack
        self.sack.load_repo(repo.get(), libdnf.rpm.PackageSack.LoadRepoFlags_USE_FILELISTS |
                            libdnf.rpm.PackageSack.LoadRepoFlags_USE_OTHER |
                            libdnf.rpm.PackageSack.LoadRepoFlags_USE_PRESTO |
                            libdnf.rpm.PackageSack.LoadRepoFlags_USE_UPDATEINFO)

    def tearDown(self):
        # Remove the cache directory.
        shutil.rmtree(self.tmpdir)

    def test_add(self):
        a = libdnf.rpm.Reldep(self.sack, "python3-labirinto = 4.2.0")
        b = libdnf.rpm.Reldep(self.sack, "(lab-list if labirinto.txt)")

        list1 = libdnf.rpm.ReldepList(self.sack)
        self.assertEqual(list1.size(), 0)
        list1.add(a)
        # TODO(jrohel): Shall we have add() with ReldepId as a part of public API?
        list1.add(b.get_id())
        list1.add_reldep("delgado > 1.2")
        self.assertEqual(list1.size(), 3)

    def test_get(self):
        a = libdnf.rpm.Reldep(self.sack, "python3-labirinto = 4.2.0")
        b = libdnf.rpm.Reldep(self.sack, "(lab-list if labirinto.txt)")
        c = libdnf.rpm.Reldep(self.sack, "(labirinto unless labirinto_c)")

        list1 = libdnf.rpm.ReldepList(self.sack)
        list1.add(a)
        list1.add(b)
        list1.add(c)

        self.assertEqual(list1.get(0), a)
        self.assertEqual(list1.get(1).to_string(), "(lab-list if labirinto.txt)")
        self.assertEqual(list1.get_id(2).id, c.get_id().id)

    def test_compare(self):
        a = libdnf.rpm.Reldep(self.sack, "python3-labirinto = 4.2.0")
        b = libdnf.rpm.Reldep(self.sack, "(lab-list if labirinto.txt)")

        list1 = libdnf.rpm.ReldepList(self.sack)
        list2 = libdnf.rpm.ReldepList(self.sack)
        self.assertEqual(list1, list2)

        list1.add(a)
        self.assertNotEqual(list1, list2)

        list2.add(a)
        list1.add(b)
        list2.add(b)
        self.assertEqual(list1, list2)

    def test_append(self):
        a = libdnf.rpm.Reldep(self.sack, "python3-labirinto = 4.2.0")
        b = libdnf.rpm.Reldep(self.sack, "(lab-list if labirinto.txt)")
        c = libdnf.rpm.Reldep(self.sack, "(labirinto unless labirinto_c)")
        d = libdnf.rpm.Reldep(self.sack, "labirinto.txt")

        list1 = libdnf.rpm.ReldepList(self.sack)
        list1.add(a)
        list1.add(b)
        list1.add_reldep("delgado > 1.2")

        list2 = libdnf.rpm.ReldepList(self.sack)
        list2.add(c)
        list2.add(d)

        list1.append(list2)
        self.assertEqual(list1.size(), 5)
        self.assertEqual(list1.get(0), a)
        self.assertEqual(list1.get(1), b)
        self.assertEqual(list1.get(2).to_string(), "delgado > 1.2")
        self.assertEqual(list1.get(3), c)
        self.assertEqual(list1.get(4), d)

    def test_iterator(self):
        a = libdnf.rpm.Reldep(self.sack, "python3-labirinto = 4.2.0")
        b = libdnf.rpm.Reldep(self.sack, "(lab-list if labirinto.txt)")
        c = libdnf.rpm.Reldep(self.sack, "(labirinto unless labirinto_c)")
        d = libdnf.rpm.Reldep(self.sack, "labirinto.txt")
        expect = [a, b, c, d]

        list1 = libdnf.rpm.ReldepList(self.sack)
        for reldep in expect:
            list1.add(reldep)

        # check if begin() points to the first Reldep
        it1 = list1.begin()
        self.assertEqual(it1.value(), a)

        # test next()
        it1.next()
        self.assertEqual(it1.value(), b)

        # check Python style iterator
        pit = iter(list1)
        self.assertEqual(next(pit), a)
        self.assertEqual(next(pit), b)
        self.assertEqual(next(pit, None), c)
        self.assertEqual(next(pit, None), d)
        self.assertEqual(next(pit, None), None)

        # test iteration using a for loop
        result = []
        for reldep in list1:
            result.append(reldep)
        self.assertEqual(result, expect)

    # add_reldep_with_glob uses libsolvs Dataiterator which needs the actual packages
    def test_add_reldep_with_glob(self):
        list1 = libdnf.rpm.ReldepList(self.sack)
        list1.add_reldep_with_glob("pkg*")

        expected = ["pkg", "pkg.conf", "pkg.conf.d", "pkg-libs", "pkg", "pkg", "pkg", "pkg", "pkg", "pkg"]
        # TODO(dmach): implement __str__()
        result = [reldep.to_string() for reldep in list1]
        self.assertEqual(expected, result)
