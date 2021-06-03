// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_PLUGIN_IPLUGIN_HPP
#define LIBDNF_PLUGIN_IPLUGIN_HPP

#include <cstdint>

namespace libdnf {

class Base;

}  // namespace libdnf

namespace libdnf::plugin {

/// Enum of all plugin hooks
enum class HookId { LOAD_CONFIG_FROM_FILE };

struct Version {
    std::uint16_t major;
    std::uint16_t minor;
    std::uint16_t micro;
};

static constexpr Version PLUGIN_API_VERSION{.major = 0, .minor = 1, .micro = 0};

class IPlugin {
public:
    IPlugin() = default;
    virtual ~IPlugin() = default;

    IPlugin(const IPlugin &) = delete;
    IPlugin(IPlugin &&) = delete;
    IPlugin & operator=(const IPlugin &) = delete;
    IPlugin & operator=(IPlugin &&) = delete;

    /// Returns the version of the API supported by the plugin. It can be called at any time.
    virtual Version get_api_version() const noexcept = 0;

    /// Returns the name of the plugin. It can be called at any time.
    virtual const char * get_name() const noexcept = 0;

    /// Gets the plugin version. It can be called at any time.
    virtual Version get_version() const noexcept = 0;

    /// Gets the value of the attribute from the plugin. Returns nullptr if the attribute does not exist.
    /// It can be called at any time.
    virtual const char * get_attribute(const char * name) const noexcept = 0;

    /// The plugin can load additional plugins. E.g. C++ plugin for loading Python plugins.
    /// Called before init.
    virtual void load_plugins(Base * /*base*/) {}

    /// Plugin initialization.
    virtual void init(Base * base) = 0;

    /// It is called when a hook is reached. The argument describes what happened.
    virtual bool hook(HookId hook_id) = 0;

    /// Finish the plugin and release all resources obtained by the init method and in hooks.
    virtual void finish() noexcept = 0;
};

}  // namespace libdnf::plugin


extern "C" {

/// Returns the version of the API supported by the plugin.
/// Same result as IPlugin::get_api_version(), but can be called without creating an IPlugin instance.
libdnf::plugin::Version libdnf_plugin_get_api_version(void);

/// Returns the name of the plugin. It can be called at any time.
/// Same result as IPlugin::get_name(), but can be called without creating an IPlugin instance.
const char * libdnf_plugin_get_name(void);

/// Returns the version of the plugin. It can be called at any time.
/// Same result as IPlugin::get_version(), but can be called without creating an IPlugin instance.
libdnf::plugin::Version libdnf_plugin_get_version(void);

/// Creates a new plugin instance. Passes the API version to the plugin.
libdnf::plugin::IPlugin * libdnf_plugin_new_instance(libdnf::plugin::Version api_version);

/// Deletes plugin instance.
void libdnf_plugin_delete_instance(libdnf::plugin::IPlugin * plugin_instance);
}

#endif
