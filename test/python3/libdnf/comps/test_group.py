# SPDX-License-Identifier: GPL-2.0-or-later
# SPDX-FileCopyrightText: Copyright contributors to the libdnf project



import os
import unittest

import libdnf.comps


class TestGroup(unittest.TestCase):
    def test_group(self):
        base = libdnf.base.Base()
        comps = libdnf.comps.Comps(base)
        reponame = "repo"
        data_path = os.path.join(os.getcwd(), "../../../test/libdnf/comps/data/core.xml")
        comps.load_from_file(data_path, reponame)
        q_core = comps.get_group_sack().new_query()
        core = q_core.get()
