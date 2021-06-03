// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_CLI_PROGRESSBAR_DOWNLOAD_PROGRESS_BAR_HPP
#define LIBDNF_CLI_PROGRESSBAR_DOWNLOAD_PROGRESS_BAR_HPP


#include "progress_bar.hpp"
#include "widgets/description.hpp"
#include "widgets/number.hpp"
#include "widgets/percent.hpp"
#include "widgets/progress.hpp"
#include "widgets/size.hpp"
#include "widgets/speed.hpp"
#include "widgets/time.hpp"


namespace libdnf::cli::progressbar {


class DownloadProgressBar : public ProgressBar {
public:
    explicit DownloadProgressBar(int64_t download_size, const std::string & description);

    using ProgressBar::get_messages;
    using ProgressBar::set_state;
    using ProgressBar::set_ticks;
    using ProgressBar::set_total_ticks;

    // TODO(dmach): add print() method

protected:
    void to_stream(std::ostream & stream) override;

private:
    // TODO(dmach): fix inconsistency - MultiProgresBar::operator<< erases previously printed lines, DownloadProgressBar::operator<< does not
    friend std::ostream & operator<<(std::ostream & stream, DownloadProgressBar & bar);

    // widgets
    libdnf::cli::progressbar::NumberWidget number_widget;
    libdnf::cli::progressbar::DescriptionWidget description_widget;
    libdnf::cli::progressbar::PercentWidget percent_widget;
    libdnf::cli::progressbar::ProgressWidget progress_widget;
    libdnf::cli::progressbar::SpeedWidget speed_widget;
    libdnf::cli::progressbar::SizeWidget size_widget;
    libdnf::cli::progressbar::TimeWidget time_widget;
};


}  // namespace libdnf::cli::progressbar


#endif  // LIBDNF_CLI_PROGRESSBAR_DOWNLOAD_PROGRESS_BAR_HPP
