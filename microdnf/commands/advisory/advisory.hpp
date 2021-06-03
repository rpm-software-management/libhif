// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef MICRODNF_COMMANDS_ADVISORY_ADVISORY_HPP
#define MICRODNF_COMMANDS_ADVISORY_ADVISORY_HPP

#include "../command.hpp"

#include <libdnf/conf/option_bool.hpp>
#include <libdnf/conf/option_enum.hpp>

#include <memory>
#include <vector>

namespace microdnf {

class CmdAdvisory : public Command {
public:
    void set_argument_parser(Context & ctx) override;
    void run(Context & ctx) override;

private:
    libdnf::OptionBool * with_cve_option{nullptr};
    libdnf::OptionBool * with_bz_option{nullptr};
    libdnf::OptionEnum<std::string> * availability_option{nullptr};
    libdnf::OptionEnum<std::string> * output_type_option{nullptr};
    std::vector<std::unique_ptr<libdnf::Option>> * patterns_to_show_options{nullptr};
};

}  // namespace microdnf

#endif
