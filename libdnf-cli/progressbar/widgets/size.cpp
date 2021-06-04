// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "common.hpp"

#include "libdnf-cli/progressbar/progress_bar.hpp"
#include "libdnf-cli/progressbar/widgets/size.hpp"


namespace libdnf::cli::progressbar {


std::size_t SizeWidget::get_total_width() const noexcept {
    return 9 + get_delimiter_before().size();
}


std::string SizeWidget::to_string() const {
    if (!get_visible()) {
        return "";
    }
    return get_delimiter_before() + format_size(get_bar()->get_ticks());
}


}  // namespace libdnf::cli::progressbar
