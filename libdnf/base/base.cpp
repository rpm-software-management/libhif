// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "libdnf/base/base.hpp"

#include "libdnf/conf/config_parser.hpp"
#include "libdnf/conf/const.hpp"

#include <fmt/format.h>

#include <algorithm>
#include <atomic>
#include <cstdlib>
#include <filesystem>
#include <mutex>
#include <vector>

namespace libdnf {

static std::atomic<Base *> locked_base{nullptr};
static std::mutex locked_base_mutex;

void Base::lock() {
    locked_base_mutex.lock();
    locked_base = this;
}

void Base::unlock() {
    if (!locked_base) {
        throw std::logic_error("Base::unlock() called on unlocked \"Base\" instance.");
    }
    if (locked_base != this) {
        throw std::logic_error("Called Base::unlock(). But the lock is not owned by this \"Base\" instance.");
    }
    locked_base = nullptr;
    locked_base_mutex.unlock();
}

Base * Base::get_locked_base() noexcept {
    return locked_base;
}

void Base::load_config_from_file(const std::string & path) {
    ConfigParser parser;
    parser.read(path);
    config.load_from_parser(parser, "main", vars, *get_logger());
}

void Base::load_config_from_file() {
    load_config_from_file(config.config_file_path().get_value());
}

void Base::load_config_from_dir(const std::string & dir_path) {
    std::vector<std::filesystem::path> paths;
    for (auto & dentry : std::filesystem::directory_iterator(dir_path)) {
        auto & path = dentry.path();
        if (path.extension() == ".conf") {
            paths.push_back(path);
        }
    }
    std::sort(paths.begin(), paths.end());
    for (auto & path : paths) {
        load_config_from_file(path);
    }
}

void Base::load_config_from_dir() {
    load_config_from_dir(libdnf::CONF_DIRECTORY);
}

void Base::add_plugin(plugin::IPlugin & iplugin_instance) {
    plugins.register_plugin(std::make_unique<plugin::Plugin>(iplugin_instance));
}

void Base::load_plugins() {
    if (const char * plugin_dir = std::getenv("LIBDNF_PLUGIN_DIR")) {
        plugins.load_plugins(plugin_dir);
    }
}

}  // namespace libdnf
