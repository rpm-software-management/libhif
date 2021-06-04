// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_CLI_PROGRESSBAR_WIDGETS_COMMON_HPP
#define LIBDNF_CLI_PROGRESSBAR_WIDGETS_COMMON_HPP


#include <cstdint>
#include <string>


namespace libdnf::cli::progressbar {


std::string format_size(int64_t num);
std::string format_time(int64_t num, bool negative);


}  // namespace libdnf::cli::progressbar


#endif  // LIBDNF_CLI_PROGRESSBAR_WIDGETS_COMMON_HPP
