// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_CLI_PROGRESSBAR_WIDGETS_PERCENT_HPP
#define LIBDNF_CLI_PROGRESSBAR_WIDGETS_PERCENT_HPP


#include "widget.hpp"


namespace libdnf::cli::progressbar {


class PercentWidget : public libdnf::cli::progressbar::Widget {
public:
    std::size_t get_total_width() const noexcept override;
    std::string to_string() const override;
};


}  // namespace libdnf::cli::progressbar


#endif  // LIBDNF_CLI_PROGRESSBAR_WIDGETS_PERCENT_HPP
