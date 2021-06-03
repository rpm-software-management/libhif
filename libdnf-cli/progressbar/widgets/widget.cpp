// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "libdnf-cli/progressbar/widgets/widget.hpp"

#include <iostream>
#include <string>


namespace libdnf::cli::progressbar {


std::ostream & operator<<(std::ostream & stream, Widget & widget) {
    stream << widget.to_string();
    return stream;
}


}  // namespace libdnf::cli::progressbar
