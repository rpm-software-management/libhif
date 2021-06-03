// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_CONF_CONFIG_HPP
#define LIBDNF_CONF_CONFIG_HPP

#include "config_parser.hpp"
#include "option_binds.hpp"
#include "option.hpp"
#include "vars.hpp"
#include "../logger/logger.hpp"

namespace libdnf {

/// Base class for configurations objects
template <Option::Priority default_priority>
class Config {
public:
    OptionBinds & opt_binds() noexcept { return binds; }

    void load_from_parser(
        const ConfigParser & parser,
        const std::string & section,
        const Vars & vars,
        Logger & logger
    );

private:
    OptionBinds binds;
};

extern template class Config<Option::Priority::MAINCONFIG>;
extern template class Config<Option::Priority::REPOCONFIG>;

}  // namespace libdnf

#endif
