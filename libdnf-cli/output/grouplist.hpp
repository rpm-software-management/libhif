// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_CLI_OUTPUT_GROUPLIST_HPP
#define LIBDNF_CLI_OUTPUT_GROUPLIST_HPP

#include "libdnf-cli/utils/tty.hpp"

#include <libsmartcols/libsmartcols.h>

namespace libdnf::cli::output {


// group list table columns
enum { COL_GROUP_ID, COL_GROUP_NAME, COL_INSTALLED };


static struct libscols_table * create_grouplist_table() {
    struct libscols_table * table = scols_new_table();
    if (isatty(1)) {
        scols_table_enable_colors(table, 1);
        scols_table_enable_maxout(table, 1);
    }
    struct libscols_column * cl = scols_table_new_column(table, "group id", 0.4, 0);
    scols_column_set_cmpfunc(cl, scols_cmpstr_cells, NULL);
    scols_table_new_column(table, "group name", 0.5, SCOLS_FL_TRUNC);
    scols_table_new_column(table, "installed", 0.1, SCOLS_FL_RIGHT);
    return table;
}


static void add_line_into_grouplist_table(
    struct libscols_table * table, const char * id, const char * name, bool installed) {
    struct libscols_line * ln = scols_table_new_line(table, NULL);
    scols_line_set_data(ln, COL_GROUP_ID, id);
    scols_line_set_data(ln, COL_GROUP_NAME, name);
    scols_line_set_data(ln, COL_INSTALLED, installed ? "yes" : "no");
    if (installed) {
        struct libscols_cell * cl = scols_line_get_cell(ln, COL_INSTALLED);
        scols_cell_set_color(cl, "green");
    }
}


void print_grouplist_table(const std::set<libdnf::comps::Group> & group_list) {
    struct libscols_table * table = create_grouplist_table();
    for (auto group: group_list) {
        add_line_into_grouplist_table(
            table,
            group.get_groupid().c_str(),
            group.get_name().c_str(),
            group.get_installed());
    }
    auto cl = scols_table_get_column(table, COL_GROUP_ID);
    scols_sort_table(table, cl);
    scols_print_table(table);
    scols_unref_table(table);
}


}  // namespace libdnf::cli::output

#endif  // LIBDNF_CLI_OUTPUT_REPOLIST_HPP
