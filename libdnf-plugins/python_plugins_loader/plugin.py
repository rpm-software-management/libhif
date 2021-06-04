# SPDX-License-Identifier: GPL-2.0-or-later
# SPDX-FileCopyrightText: Copyright contributors to the libdnf project


import libdnf


class Plugin(libdnf.plugin.IPlugin):
    def __init__(self):
        super(Plugin, self).__init__()
        self.base = None

    @staticmethod
    def get_api_version():
        return libdnf.plugin.Version(0, 1, 0)

    @staticmethod
    def get_name():
        return 'python_test_plugin'

    @staticmethod
    def get_version():
        return libdnf.plugin.Version(0, 1, 0)

    def get_attribute(self, name):
        attributes = {'author_name': 'Jaroslav Rohel',
                      'author_email': 'jrohel@redhat.com',
                      'description': 'Libdnf plugin written in Python. Processes the LOAD_CONFIG_FROM_FILE hook '
                                     'and writes the current value of the "skip_if_unavailable" option to the log.'}
        return attributes.get(name, None)

    def init(self, base):
        self.base = base

    def hook(self, plugin_hook_id):
        if plugin_hook_id == libdnf.plugin.HookId_LOAD_CONFIG_FROM_FILE:
            logger = self.base.get_logger()
            config = self.base.get_config()
            logger.info(self.get_name() + ' - skip_if_unavailable = ' + str(config.skip_if_unavailable().get_value()))
        return True

    def finish(self):
        pass
