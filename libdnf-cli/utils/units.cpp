// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "units.hpp"

#include <cstring>
#include <fmt/format.h>


namespace libdnf::cli::utils::units {


static const char * const SIZE_UNITS[] = {
    "B",
    "KiB",
    "MiB",
    "GiB",
    "TiB",
    "PiB",
    "EiB",
    "ZiB",
    "YiB",
};


std::string format_size(int64_t num) {
    auto i = static_cast<float>(num);
    int index = 0;
    while (i > 999) {
        i /= 1024;
        index++;
    }
    return fmt::format("{0:.1f} {1:>3s}", i, SIZE_UNITS[index]);
}


}  // namespace libdnf::cli::utils::units
