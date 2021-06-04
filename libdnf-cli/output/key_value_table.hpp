// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_CLI_OUTPUT_KEY_VALUE_TABLE_HPP
#define LIBDNF_CLI_OUTPUT_KEY_VALUE_TABLE_HPP

#include <libsmartcols/libsmartcols.h>

#include <string>
#include <vector>


namespace libdnf::cli::output {


class KeyValueTable {
public:
    explicit KeyValueTable();
    ~KeyValueTable();
    void print();

protected:
    struct libscols_line * add_line(
        const char * key,
        const char * value,
        const char * color = nullptr,
        struct libscols_line * parent = nullptr
    );

    struct libscols_line * add_line(
        const char * key,
        const std::string & value,
        const char * color = nullptr,
        struct libscols_line * parent = nullptr
    );

    struct libscols_line * add_line(
        const char * key,
        const std::vector<std::string> & value,
        const char * color = nullptr,
        struct libscols_line * parent = nullptr
    );

    template<typename V>
    struct libscols_line * add_line(
        const char * key,
        V value,
        const char * color = nullptr,
        struct libscols_line * parent = nullptr
    ) {
        return add_line(key, std::to_string(value), color, parent);
    }

private:
    struct libscols_table * tb = nullptr;
};


}  // namespace libdnf::cli::output


#endif  // LIBDNF_CLI_OUTPUT_KEY_VALUE_TABLE_HPP
