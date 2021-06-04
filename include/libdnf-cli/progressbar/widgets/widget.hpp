// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_CLI_PROGRESSBAR_WIDGETS_WIDGET_HPP
#define LIBDNF_CLI_PROGRESSBAR_WIDGETS_WIDGET_HPP


#include <string>


namespace libdnf::cli::progressbar {


class ProgressBar;


class Widget {
public:
    bool get_visible() const noexcept { return visible; }
    void set_visible(bool value) { visible = value; };

    ProgressBar * get_bar() const noexcept { return bar; }
    void set_bar(ProgressBar * value) { bar = value; }

    virtual std::size_t get_total_width() const noexcept = 0;
    virtual std::string to_string() const = 0;

    const std::string & get_delimiter_before() const noexcept { return delimiter_before; }
    void set_delimiter_before(const std::string & value) { delimiter_before = value; }

private:
    ProgressBar * bar = nullptr;
    bool visible{true};
    std::string delimiter_before;
};


}  // namespace libdnf::cli::progressbar


#endif  // LIBDNF_CLI_PROGRESSBAR_WIDGETS_WIDGET_HPP
