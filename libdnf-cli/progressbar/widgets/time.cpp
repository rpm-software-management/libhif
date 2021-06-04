// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "common.hpp"

#include "libdnf-cli/progressbar/progress_bar.hpp"
#include "libdnf-cli/progressbar/widgets/time.hpp"

#include <cmath>
#include <iomanip>
#include <sstream>


namespace libdnf::cli::progressbar {


std::size_t TimeWidget::get_total_width() const noexcept {
    return 7 + get_delimiter_before().size();
}


std::string TimeWidget::to_string() const {
    if (!get_visible()) {
        return "";
    }
    std::ostringstream ss;
    ss << get_delimiter_before();
    if (get_bar()->is_finished() || get_bar()->get_total_ticks() < 0) {
        // finshed or unknown total ticks -> display elapsed time
        ss << format_time(get_bar()->get_elapsed_seconds(), false);
    } else {
        // in progress -> display remaining time
        ss << format_time(get_bar()->get_remaining_seconds(), true);
    }
    return ss.str();
}


}  // namespace libdnf::cli::progressbar
