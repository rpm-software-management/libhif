// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_CLI_PROGRESSBAR_MULTI_PROGRESS_BAR_HPP
#define LIBDNF_CLI_PROGRESSBAR_MULTI_PROGRESS_BAR_HPP


#include "download_progress_bar.hpp"
#include "progress_bar.hpp"

#include <iostream>
#include <vector>


namespace libdnf::cli::progressbar {


class MultiProgressBar {
public:
    explicit MultiProgressBar();
    ~MultiProgressBar();

    // TODO(dmach): use std::unique_ptr instead of the raw pointer?
    void add_bar(ProgressBar * bar);
    void print() { std::cout << *this; std::cout << std::flush; }
    friend std::ostream & operator<<(std::ostream & stream, MultiProgressBar & mbar);

private:
    std::vector<ProgressBar *> bars_all;
    std::vector<ProgressBar *> bars_todo;
    std::vector<ProgressBar *> bars_done;
    DownloadProgressBar total;
    std::size_t printed_lines = 0;
};


}  // namespace libdnf::cli::progressbar


#endif  // LIBDNF_CLI_PROGRESSBAR_MULTI_PROGRESS_BAR_HPP
