// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "libdnf/conf/config.hpp"

#include <fmt/format.h>

namespace libdnf {

template <Option::Priority default_priority>
void Config<default_priority>::load_from_parser(
    const ConfigParser & parser,
    const std::string & section,
    const Vars & vars,
    Logger & logger
) {
    auto cfg_parser_data_iter = parser.get_data().find(section);
    if (cfg_parser_data_iter != parser.get_data().end()) {
        for (const auto & opt : cfg_parser_data_iter->second) {
            auto opt_binds_iter = binds.find(opt.first);
            if (opt_binds_iter != binds.end()) {
                try {
                    opt_binds_iter->second.new_string(default_priority, vars.substitute(opt.second));
                } catch (const Option::Exception & ex) {
                    logger.warning(fmt::format(
                        R"**(Config error in section "{}" key "{}": {}: {})**",
                        section,
                        opt.first,
                        ex.get_description(),
                        ex.what()
                    ));
                }
            }
        }
    }
}

template class Config<Option::Priority::MAINCONFIG>;
template class Config<Option::Priority::REPOCONFIG>;

}
