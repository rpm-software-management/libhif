// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_CLI_UTILS_TTY_TTY_HPP
#define LIBDNF_CLI_UTILS_TTY_TTY_HPP


#include <iostream>


namespace libdnf::cli::utils::tty {


std::ostream & reset(std::ostream & stream);

std::ostream & bold(std::ostream & stream);
std::ostream & underline(std::ostream & stream);
std::ostream & blink(std::ostream & stream);

std::ostream & black(std::ostream & stream);
std::ostream & red(std::ostream & stream);
std::ostream & green(std::ostream & stream);
std::ostream & yellow(std::ostream & stream);
std::ostream & blue(std::ostream & stream);
std::ostream & magenta(std::ostream & stream);
std::ostream & cyan(std::ostream & stream);
std::ostream & white(std::ostream & stream);

std::ostream & clear_line(std::ostream & stream);
std::ostream & cursor_up(std::ostream & stream);

std::ostream & cursor_hide(std::ostream & stream);
std::ostream & cursor_show(std::ostream & stream);


int get_width();
bool is_interactive();


}  // namespace libdnf::cli::utils::tty


#endif  // LIBDNF_CLI_UTILS_TTY_TTY_HPP
