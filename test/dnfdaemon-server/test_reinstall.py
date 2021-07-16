# Copyright (C) 2021 Red Hat, Inc.
#
# This file is part of dnfdaemon-server: https://github.com/rpm-software-management/libdnf/
#
# Dnfdaemon-server is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# Dnfdaemon-server is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with dnfdaemon-server.  If not, see <https://www.gnu.org/licenses/>.

import dbus
import os
import subprocess

import support

class ReinstallTest(support.InstallrootCase):

    def setUp(self):
        super(ReinstallTest, self).setUp()
        # install a package inside the installroot
        pkg_file = os.path.join(support.PROJECT_BINARY_DIR, "test/data/repos-rpm/rpm-repo1/one-1-1.noarch.rpm")
        res = subprocess.run(["rpm", "--root", self.installroot, "-U", pkg_file])
        self.assertEqual(res.returncode, 0, "Installation of test package '{}' failed.".format(pkg_file))

    def test_reinstall_package(self):
        # remove an installed package
        self.iface_rpm.reinstall(['one'], dbus.Dictionary({}, signature='sv'))

        resolved = self.iface_goal.resolve(dbus.Dictionary({}, signature='sv'))

        # id of package depends on order of the repos in the sack which varies
        # between runs so we can't rely on the value
        for action, pkg in resolved:
            pkg.pop('id')
            pkg.pop('package_size')

        self.assertCountEqual(
            resolved,
            dbus.Array([
                dbus.Struct((
                    dbus.UInt32(9),   # action reinstall
                    dbus.Dictionary({ # package
                        dbus.String('evr'): dbus.String('1-1', variant_level=1),
                        dbus.String('name'): dbus.String('one', variant_level=1),
                        dbus.String('epoch'): dbus.String('0', variant_level=1),
                        dbus.String('version'): dbus.String('1', variant_level=1),
                        dbus.String('release'): dbus.String('1', variant_level=1),
                        dbus.String('arch'): dbus.String('noarch', variant_level=1),
                        dbus.String('install_size'): dbus.UInt64(0, variant_level=1),
                        dbus.String('repo'): dbus.String('rpm-repo1', variant_level=1),
                        }, signature=dbus.Signature('sv'))),
                    signature=None),
                dbus.Struct((
                    dbus.UInt32(10),   # action reinstalled
                    dbus.Dictionary({ # package
                        dbus.String('evr'): dbus.String('1-1', variant_level=1),
                        dbus.String('name'): dbus.String('one', variant_level=1),
                        dbus.String('epoch'): dbus.String('0', variant_level=1),
                        dbus.String('version'): dbus.String('1', variant_level=1),
                        dbus.String('release'): dbus.String('1', variant_level=1),
                        dbus.String('arch'): dbus.String('noarch', variant_level=1),
                        dbus.String('install_size'): dbus.UInt64(0, variant_level=1),
                        dbus.String('repo'): dbus.String('@System', variant_level=1),
                        }, signature=dbus.Signature('sv'))),
                    signature=None)
                ], signature=dbus.Signature('(ua{sv})'))
            )

        self.iface_goal.do_transaction(dbus.Dictionary({}, signature='sv'))

