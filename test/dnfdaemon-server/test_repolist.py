# SPDX-License-Identifier: GPL-2.0-or-later
# SPDX-FileCopyrightText: Copyright contributors to the libdnf project


import dbus
import os
import shutil
import tempfile
import unittest

import support

class RepoTest(unittest.TestCase):

    def setUp(self):
        self.installroot = tempfile.mkdtemp(prefix="dnfdaemon-test-")
        self.reposdir = os.path.join(support.PROJECT_BINARY_DIR, "test/data/repos-rpm-conf.d")

        self.bus = dbus.SystemBus()
        self.iface_session = dbus.Interface(
            self.bus.get_object(support.DNFDAEMON_BUS_NAME, support.DNFDAEMON_OBJECT_PATH),
            dbus_interface=support.IFACE_SESSION_MANAGER)
        self.session = self.iface_session.open_session({
            "installroot": self.installroot,
            "cachedir": os.path.join(self.installroot, "var/cache/dnf"),
            "reposdir": self.reposdir,
        })
        self.iface_repo = dbus.Interface(
            self.bus.get_object(support.DNFDAEMON_BUS_NAME, self.session),
            dbus_interface=support.IFACE_REPO)

    def tearDown(self):
        self.iface_session.close_session(self.session)
        shutil.rmtree(self.installroot)

    def test_list_repos(self):
        # get list of all repositories
        self.assertEqual(
            self.iface_repo.list({"repo_attrs": ["name", "enabled"]}),
            dbus.Array([
                dbus.Dictionary({
                    dbus.String('name'): dbus.String('Repository 1', variant_level=1),
                    dbus.String('enabled'): dbus.Boolean(True, variant_level=1),
                    dbus.String('id'): dbus.String('rpm-repo1', variant_level=1)},
                    signature=dbus.Signature('sv')),
                dbus.Dictionary({
                    dbus.String('name'): dbus.String('Repository 2', variant_level=1),
                    dbus.String('enabled'): dbus.Boolean(True, variant_level=1),
                    dbus.String('id'): dbus.String('rpm-repo2', variant_level=1)},
                    signature=dbus.Signature('sv')),
                ],
                signature=dbus.Signature('a{sv}'))
        )


    def test_list_repos_spec(self):
        # get list of specified repositories
        self.assertEqual(
            self.iface_repo.list(
                    {"repo_attrs": ["name", "enabled"], "patterns": ['rpm-repo1']}),
            dbus.Array([
                dbus.Dictionary({
                    dbus.String('name'): dbus.String('Repository 1', variant_level=1),
                    dbus.String('enabled'): dbus.Boolean(True, variant_level=1),
                    dbus.String('id'): dbus.String('rpm-repo1', variant_level=1)},
                    signature=dbus.Signature('sv')),
                ],
                signature=dbus.Signature('a{sv}'))
        )
