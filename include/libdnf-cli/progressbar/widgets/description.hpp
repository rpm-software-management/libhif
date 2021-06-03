// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_CLI_PROGRESSBAR_WIDGETS_DESCRIPTION_HPP
#define LIBDNF_CLI_PROGRESSBAR_WIDGETS_DESCRIPTION_HPP


#include "widget.hpp"


namespace libdnf::cli::progressbar {


class DescriptionWidget : public Widget {
public:
    std::size_t get_total_width() const noexcept override;

    void set_total_width(std::size_t value);
    void set_width(std::size_t value) { width = value; }

    std::string to_string() const override;

private:
    std::size_t width = 0;
};


}  // namespace libdnf::cli::progressbar


#endif  // LIBDNF_CLI_PROGRESSBAR_WIDGETS_DESCRIPTION_HPP
