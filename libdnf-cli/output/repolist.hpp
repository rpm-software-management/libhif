// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_CLI_OUTPUT_REPOLIST_HPP
#define LIBDNF_CLI_OUTPUT_REPOLIST_HPP

#include "libdnf-cli/utils/tty.hpp"

#include <libsmartcols/libsmartcols.h>

namespace libdnf::cli::output {

// repository list table columns
enum { COL_REPO_ID, COL_REPO_NAME, COL_REPO_STATUS };

static struct libscols_table * create_repolist_table(bool with_status) {
    struct libscols_table * table = scols_new_table();
    if (libdnf::cli::utils::tty::is_interactive()) {
        scols_table_enable_colors(table, 1);
        scols_table_enable_maxout(table, 1);
    }
    struct libscols_column * cl = scols_table_new_column(table, "repo id", 0.4, 0);
    scols_column_set_cmpfunc(cl, scols_cmpstr_cells, NULL);
    scols_table_new_column(table, "repo name", 0.5, SCOLS_FL_TRUNC);
    if (with_status) {
        scols_table_new_column(table, "status", 0.1, SCOLS_FL_RIGHT);
    }
    return table;
}

static void add_line_into_repolist_table(
    struct libscols_table * table, bool with_status, const char * id, const char * descr, bool enabled) {
    struct libscols_line * ln = scols_table_new_line(table, NULL);
    scols_line_set_data(ln, COL_REPO_ID, id);
    scols_line_set_data(ln, COL_REPO_NAME, descr);
    if (with_status) {
        scols_line_set_data(ln, COL_REPO_STATUS, enabled ? "enabled" : "disabled");
        struct libscols_cell * cl = scols_line_get_cell(ln, COL_REPO_STATUS);
        scols_cell_set_color(cl, enabled ? "green" : "red");
    }
}

template <class Query>
static void print_repolist_table(Query query, bool with_status, size_t sort_column) {
    auto table = create_repolist_table(with_status);
    for (auto & repo : query.get_data()) {
        add_line_into_repolist_table(
            table,
            with_status,
            repo->get_id().c_str(),
            repo->get_name().c_str(), //repo->get_config().name().get_value().c_str(),
            repo->is_enabled());
    }
    auto cl = scols_table_get_column(table, sort_column);
    scols_sort_table(table, cl);

    scols_print_table(table);
    scols_unref_table(table);
}


}  // namespace libdnf::cli::output

#endif  // LIBDNF_CLI_OUTPUT_REPOLIST_HPP
