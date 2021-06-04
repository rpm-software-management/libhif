// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_RPM_CONFIG_REPO_HPP
#define LIBDNF_RPM_CONFIG_REPO_HPP

#include "libdnf/conf/config_main.hpp"
#include "libdnf/conf/option_child.hpp"

#include <memory>

namespace libdnf::repo {

/// Holds repo configuration options. Default values of some options are inherited from ConfigMain.
class ConfigRepo : public Config<Option::Priority::REPOCONFIG> {
public:
    explicit ConfigRepo(ConfigMain & main_config);
    ~ConfigRepo();
    ConfigRepo(ConfigRepo && src);

    ConfigMain & get_main_config();
    const ConfigMain & get_main_config() const;

    OptionString & name();
    const OptionString & name() const;
    OptionChild<OptionBool> & enabled();
    const OptionChild<OptionBool> & enabled() const;
    OptionChild<OptionString> & basecachedir();
    const OptionChild<OptionString> & basecachedir() const;
    OptionStringList & baseurl();
    const OptionStringList & baseurl() const;
    OptionString & mirrorlist();
    const OptionString & mirrorlist() const;
    OptionString & metalink();
    const OptionString & metalink() const;
    OptionString & type();
    const OptionString & type() const;
    OptionString & mediaid();
    const OptionString & mediaid() const;
    OptionStringList & gpgkey();
    const OptionStringList & gpgkey() const;
    OptionStringList & excludepkgs();
    const OptionStringList & excludepkgs() const;
    OptionStringList & includepkgs();
    const OptionStringList & includepkgs() const;
    OptionChild<OptionBool> & fastestmirror();
    const OptionChild<OptionBool> & fastestmirror() const;
    OptionChild<OptionString> & proxy();
    const OptionChild<OptionString> & proxy() const;
    OptionChild<OptionString> & proxy_username();
    const OptionChild<OptionString> & proxy_username() const;
    OptionChild<OptionString> & proxy_password();
    const OptionChild<OptionString> & proxy_password() const;
    OptionChild<OptionEnum<std::string>> & proxy_auth_method();
    const OptionChild<OptionEnum<std::string>> & proxy_auth_method() const;
    OptionChild<OptionString> & username();
    const OptionChild<OptionString> & username() const;
    OptionChild<OptionString> & password();
    const OptionChild<OptionString> & password() const;
    OptionChild<OptionStringList> & protected_packages();
    const OptionChild<OptionStringList> & protected_packages() const;
    OptionChild<OptionBool> & gpgcheck();
    const OptionChild<OptionBool> & gpgcheck() const;
    OptionChild<OptionBool> & repo_gpgcheck();
    const OptionChild<OptionBool> & repo_gpgcheck() const;
    OptionChild<OptionBool> & enablegroups();
    const OptionChild<OptionBool> & enablegroups() const;
    OptionChild<OptionNumber<std::uint32_t>> & retries();
    const OptionChild<OptionNumber<std::uint32_t>> & retries() const;
    OptionChild<OptionNumber<std::uint32_t>> & bandwidth();
    const OptionChild<OptionNumber<std::uint32_t>> & bandwidth() const;
    OptionChild<OptionNumber<std::uint32_t>> & minrate();
    const OptionChild<OptionNumber<std::uint32_t>> & minrate() const;
    OptionChild<OptionEnum<std::string>> & ip_resolve();
    const OptionChild<OptionEnum<std::string>> & ip_resolve() const;
    OptionChild<OptionNumber<float>> & throttle();
    const OptionChild<OptionNumber<float>> & throttle() const;
    OptionChild<OptionSeconds> & timeout();
    const OptionChild<OptionSeconds> & timeout() const;
    OptionChild<OptionNumber<std::uint32_t>> & max_parallel_downloads();
    const OptionChild<OptionNumber<std::uint32_t>> & max_parallel_downloads() const;
    OptionChild<OptionSeconds> & metadata_expire();
    const OptionChild<OptionSeconds> & metadata_expire() const;
    OptionNumber<std::int32_t> & cost();
    const OptionNumber<std::int32_t> & cost() const;
    OptionNumber<std::int32_t> & priority();
    const OptionNumber<std::int32_t> & priority() const;
    OptionBool & module_hotfixes();
    const OptionBool & module_hotfixes() const;
    OptionChild<OptionString> & sslcacert();
    const OptionChild<OptionString> & sslcacert() const;
    OptionChild<OptionBool> & sslverify();
    const OptionChild<OptionBool> & sslverify() const;
    OptionChild<OptionString> & sslclientcert();
    const OptionChild<OptionString> & sslclientcert() const;
    OptionChild<OptionString> & sslclientkey();
    const OptionChild<OptionString> & sslclientkey() const;
    OptionChild<OptionString> & proxy_sslcacert();
    const OptionChild<OptionString> & proxy_sslcacert() const;
    OptionChild<OptionBool> & proxy_sslverify();
    const OptionChild<OptionBool> & proxy_sslverify() const;
    OptionChild<OptionString> & proxy_sslclientcert();
    const OptionChild<OptionString> & proxy_sslclientcert() const;
    OptionChild<OptionString> & proxy_sslclientkey();
    const OptionChild<OptionString> & proxy_sslclientkey() const;
    OptionChild<OptionBool> & deltarpm();
    const OptionChild<OptionBool> & deltarpm() const;
    OptionChild<OptionNumber<std::uint32_t>> & deltarpm_percentage();
    const OptionChild<OptionNumber<std::uint32_t>> & deltarpm_percentage() const;
    OptionChild<OptionBool> & skip_if_unavailable();
    const OptionChild<OptionBool> & skip_if_unavailable() const;
    /// If true it will create libsolv cache that will speed up the next loading process
    OptionBool & build_cache();
    const OptionBool & build_cache() const;

    // option recognized by other tools, e.g. gnome-software, but unused in dnf
    OptionString & enabled_metadata();
    const OptionString & enabled_metadata() const;

    OptionChild<OptionString> & user_agent();
    const OptionChild<OptionString> & user_agent() const;
    OptionChild<OptionBool> & countme();
    const OptionChild<OptionBool> & countme() const;
    // yum compatibility options
    OptionEnum<std::string> & failovermethod();
    const OptionEnum<std::string> & failovermethod() const;

private:
    class Impl;
    std::unique_ptr<Impl> p_impl;
};

}  // namespace libdnf::repo

#endif
