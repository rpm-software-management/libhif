// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_BASE_BASE_HPP
#define LIBDNF_BASE_BASE_HPP

#include "libdnf/advisory/advisory_sack.hpp"
#include "libdnf/common/weak_ptr.hpp"
#include "libdnf/comps/comps.hpp"
#include "libdnf/conf/config_main.hpp"
#include "libdnf/conf/vars.hpp"
#include "libdnf/logger/log_router.hpp"
#include "libdnf/plugin/plugins.hpp"
#include "libdnf/rpm/package_sack.hpp"
#include "libdnf/repo/repo_sack.hpp"
#include "libdnf/transaction/sack.hpp"

#include <map>

namespace libdnf {

using LogRouterWeakPtr = WeakPtr<LogRouter, false>;
using VarsWeakPtr = WeakPtr<Vars, false>;

/// Instances of :class:`libdnf::Base` are the central point of functionality supplied by libdnf.
/// An application will typically create a single instance of this class which it will keep for the run-time needed to accomplish its packaging tasks.
/// :class:`.Base` instances are stateful objects owning various data.
class Base {
public:
    /// Sets the pointer to the locked instance "Base" to "this" instance. Blocks if the pointer is already set.
    /// Pointer to a locked "Base" instance can be obtained using "get_locked_base()".
    void lock();

    /// Resets the pointer to a locked "Base" instance to "nullptr".
    /// Throws an exception if another or no instance is locked.
    void unlock();

    /// Returns a pointer to a locked "Base" instance or "nullptr" if no instance is locked.
    static Base * get_locked_base() noexcept;

    /// Loads main configuration from file defined by the current configuration.
    void load_config_from_file();

    ConfigMain & get_config() { return config; }
    LogRouterWeakPtr get_logger() { return LogRouterWeakPtr(&log_router, &log_router_gurad); }
    repo::RepoSackWeakPtr get_rpm_repo_sack() { return rpm_repo_sack.get_weak_ptr(); }
    rpm::PackageSackWeakPtr get_rpm_package_sack() { return rpm_package_sack.get_weak_ptr(); }

    transaction::TransactionSackWeakPtr get_transaction_sack() { return transaction_sack.get_weak_ptr(); }
    libdnf::comps::CompsWeakPtr get_comps() { return comps.get_weak_ptr(); }
    libdnf::advisory::AdvisorySackWeakPtr get_rpm_advisory_sack() { return rpm_advisory_sack.get_weak_ptr(); }

    /// Gets base variables. They can be used in configuration files. Syntax in the config - ${var_name} or $var_name.
    VarsWeakPtr get_vars() { return VarsWeakPtr(&vars, &vars_gurad); }

    void add_plugin(plugin::IPlugin & iplugin_instance);
    void load_plugins();
    plugin::Plugins & get_plugins() { return plugins; }

private:
    //TODO(jrohel): Make public?
    /// Loads main configuration from file defined by path.
    void load_config_from_file(const std::string & path);

    //TODO(jrohel): Make public? Will we support drop-in configuration directories?
    /// Loads main configuration from files with ".conf" extension from directory defined by dir_path.
    /// The files in the directory are read in alphabetical order.
    void load_config_from_dir(const std::string & dir_path);

    //TODO(jrohel): Make public? Will we support drop-in configuration directories?
    /// Loads main configuration from files with ".conf" extension from directory defined by the current configuration.
    /// The files in the directory are read in alphabetical order.
    void load_config_from_dir();

    ConfigMain config;
    LogRouter log_router;
    repo::RepoSack rpm_repo_sack{*this};
    rpm::PackageSack rpm_package_sack{*this};
    transaction::TransactionSack transaction_sack{*this};
    comps::Comps comps{*this};
    Vars vars;
    plugin::Plugins plugins{*this};
    libdnf::advisory::AdvisorySack rpm_advisory_sack{*this};
    std::map<std::string, std::string> variables;

    WeakPtrGuard<LogRouter, false> log_router_gurad;
    WeakPtrGuard<Vars, false> vars_gurad;
};

}  // namespace libdnf

#endif
