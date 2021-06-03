// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "common.hpp"

#include "libdnf-cli/progressbar/progress_bar.hpp"
#include "libdnf-cli/progressbar/widgets/number.hpp"

#include <cmath>
#include <iomanip>
#include <sstream>


namespace libdnf::cli::progressbar {


std::size_t NumberWidget::get_total_width() const noexcept {
    return 3 + (2 * get_number_width()) + get_delimiter_before().size();
}


std::size_t NumberWidget::get_number_width() const {
    std::size_t result = std::max(
        static_cast<std::size_t>(log10(get_bar()->get_number())),
        static_cast<std::size_t>(log10(get_bar()->get_total())));
    return result + 1;
}


std::string NumberWidget::to_string() const {
    if (!get_visible()) {
        return "";
    }
    std::ostringstream ss;
    auto number_width = get_number_width();
    ss << get_delimiter_before();
    ss << "[";
    ss << std::setw(static_cast<int32_t>(number_width)) << get_bar()->get_number();
    ss << "/";
    ss << std::setw(static_cast<int32_t>(number_width)) << get_bar()->get_total();
    ss << "]";
    return ss.str();
}


}  // namespace libdnf::cli::progressbar
