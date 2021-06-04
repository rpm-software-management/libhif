// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "libdnf-cli/progressbar/progress_bar.hpp"


namespace libdnf::cli::progressbar {


ProgressBar::ProgressBar(int64_t total_ticks) : total_ticks{total_ticks} {}


ProgressBar::ProgressBar(int64_t total_ticks, const std::string & description)
    : total_ticks{total_ticks}
    , description{description} {}


void ProgressBar::reset() {
    ticks = 0;
    total_ticks = 0;
    number = 0;
    total = 0;
    description = "";
    messages.clear();
    state = ProgressBarState::READY;
    percent_done = 0;
    elapsed_seconds = 0;
    remaining_seconds = 0;
    average_speed = 0;
    auto_finish = true;
    begin = std::chrono::system_clock::from_time_t(0);
    end = std::chrono::system_clock::from_time_t(0);
    current_speed_window_start = std::chrono::system_clock::now();
    current_speed = 0;
    current_speed_window_ticks = 0;
}


void ProgressBar::set_ticks(int64_t value) {
    if (is_finished()) {
        return;
    }
    auto new_ticks = value;
    if (total_ticks >= 0) {
        new_ticks = std::min(new_ticks, total_ticks);
    }
    if (new_ticks >= ticks) {
        current_speed_window_ticks += new_ticks - ticks;
    } else {
        current_speed_window_ticks = 0;
    }
    ticks = new_ticks;
}


void ProgressBar::add_ticks(int64_t value) {
    set_ticks(ticks + value);
}


void ProgressBar::start() {
    if (begin == std::chrono::system_clock::from_time_t(0)) {
        begin = std::chrono::system_clock::now();
        state = ProgressBarState::STARTED;
    }
}


void ProgressBar::update() {
    if (is_finished()) {
        return;
    }

    if (ticks >= total_ticks) {
        percent_done = 100;
    } else if (total_ticks == 0) {
        // can't divide by zero, consider progressbar 100% complete
        percent_done = 100;
    } else if (total_ticks > 0) {
        percent_done = static_cast<int32_t>(static_cast<float>(ticks) / static_cast<float>(total_ticks) * 100);
    } else {
        // unknown total ticks
        percent_done = -1;
    }

    auto now = std::chrono::high_resolution_clock::now();

    // compute the current speed (ticks per second)
    auto delta = now - current_speed_window_start;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(delta).count();

    if (ms > 950) {
        current_speed = current_speed_window_ticks * 1000 / ms;
        // reset the window
        current_speed_window_ticks = 0;
        current_speed_window_start = now;
    } else if (current_speed == 0 && ms != 0) {
        current_speed = current_speed_window_ticks * 1000 / ms;
    }

    // compute average speed
    delta = now - begin;
    ms = std::chrono::duration_cast<std::chrono::milliseconds>(delta).count();
    if (ms == 0) {
        average_speed = 0;
    } else {
        average_speed = ticks * 1000 / ms;
    }

    // compute elapsed seconds
    // round the result to display 00m00s less frequently
    elapsed_seconds = std::chrono::round<std::chrono::seconds>(delta).count();

    // compute remaining time
    if (total_ticks >= 0) {
        int64_t remaining_ticks = total_ticks - ticks;
        if (current_speed != 0) {
            remaining_seconds = remaining_ticks / current_speed;
        }
    } else {
        // unknown total ticks
        remaining_seconds = -1;
    }

    if (total_ticks >= 0 && ticks >= total_ticks) {
        if (auto_finish && state == ProgressBarState::STARTED) {
            // avoid calling set_state() because it triggers update() and ends up in an endless recursion
            state = ProgressBarState::SUCCESS;
        }
        end = now;
        percent_done = 100;
        remaining_seconds = 0;
    }
}


void ProgressBar::set_total_ticks(int64_t value) {
    if (is_finished()) {
        return;
    }
    total_ticks = value;
}


std::ostream & operator<<(std::ostream & os, ProgressBar & bar) {
    bar.to_stream(os);
    return os;
}


}  // namespace libdnf::cli::progressbar
