// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "libdnf-cli/progressbar/progress_bar.hpp"
#include "libdnf-cli/progressbar/widgets/percent.hpp"

#include <cmath>
#include <iomanip>
#include <sstream>


namespace libdnf::cli::progressbar {


std::size_t PercentWidget::get_total_width() const noexcept {
    return 4 + get_delimiter_before().size();
}


std::string PercentWidget::to_string() const {
    if (!get_visible()) {
        return "";
    }

    std::ostringstream ss;
    ss << get_delimiter_before();
    if (get_bar()->get_percent_done() >= 0) {
        ss << std::right << std::setw(3) << get_bar()->get_percent_done();
    } else {
        ss << std::right << std::setw(3) << "???";
    }
    ss << "%";
    return ss.str();
}


}  // namespace libdnf::cli::progressbar
