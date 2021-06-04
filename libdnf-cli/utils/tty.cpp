// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "tty.hpp"

#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include <cstdlib>


namespace libdnf::cli::utils::tty {


int get_width() {
    // Use a custom "FORCE_COLUMNS" variable for testing purposes.
    // "COLUMNS" is overwritten in a sub-shell and that makes testing more difficult
    char * columns = std::getenv("FORCE_COLUMNS");
    if (columns != nullptr) {
        try {
            return std::stoi(columns);
        } catch (std::invalid_argument & ex) {
        } catch (std::out_of_range & ex) {
        }
    }

    struct winsize size;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == 0) {
        return size.ws_col;
    }

    return 80;
}


bool is_interactive() {
    return isatty(fileno(stdout)) == 1;
}


#define TTY_COMMAND(name, escape_code)           \
    std::ostream & name(std::ostream & stream) { \
        if (is_interactive()) {                  \
            stream << escape_code;               \
        }                                        \
        return stream;                           \
    }


// tty::reset
TTY_COMMAND(reset, "\033[0m")

// tty::bold
TTY_COMMAND(bold, "\033[1m")

// tty::underline
TTY_COMMAND(underline, "\033[4m")

// tty::blink
TTY_COMMAND(blink, "\033[5m")

// tty::black
TTY_COMMAND(black, "\033[30m")

// tty::red
TTY_COMMAND(red, "\033[31m")

// tty::green
TTY_COMMAND(green, "\033[32m")

// tty::yellow
TTY_COMMAND(yellow, "\033[33m")

// tty::blue
TTY_COMMAND(blue, "\033[34m")

// tty::magenta
TTY_COMMAND(magenta, "\033[35m")

// tty::cyan
TTY_COMMAND(cyan, "\033[36m")

// tty::white
TTY_COMMAND(white, "\033[37m")

// tty::clear_line
TTY_COMMAND(clear_line, "\033[2K")

// tty::cursor_up
TTY_COMMAND(cursor_up, "\x1b[A")

// tty::cursor_hide
TTY_COMMAND(cursor_hide, "\x1b[?25l")

// tty::cursor_show
TTY_COMMAND(cursor_show, "\x1b[?25h")


}  // namespace libdnf::cli::utils::tty
