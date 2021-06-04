// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "common.hpp"

#include "libdnf-cli/progressbar/progress_bar.hpp"
#include "libdnf-cli/progressbar/widgets/description.hpp"

#include <iomanip>


namespace libdnf::cli::progressbar {


std::size_t DescriptionWidget::get_total_width() const noexcept {
    if (width > 0) {
        return width + get_delimiter_before().size();
    }
    return get_bar()->get_description().size() + get_delimiter_before().size();
}


void DescriptionWidget::set_total_width(std::size_t value) {
    set_width(value - get_delimiter_before().size());
}


std::string DescriptionWidget::to_string() const {
    if (!get_visible()) {
        return "";
    }
    std::ostringstream ss;
    ss << get_delimiter_before();
    ss << std::left;
    ss << std::setw(static_cast<int>(get_total_width() - get_delimiter_before().size()));
    ss << get_bar()->get_description();
    return ss.str();
}


}  // namespace libdnf::cli::progressbar
