// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "db.hpp"

#include "libdnf/base/base.hpp"

#include <filesystem>


namespace libdnf::transaction {


static const char * SQL_CREATE_TABLES =
#include "sql/create_tables.sql"
    ;


static const char * SQL_TABLE_CONFIG_EXISTS = R"**(
    SELECT
        name
    FROM
        sqlite_master
    WHERE
        type='table'
        AND name='config'
)**";


static const char * SQL_GET_SCHEMA_VERSION = R"**(
    SELECT
        value
    FROM
        config
    WHERE
        key = 'version'
)**";


void transaction_db_create(libdnf::utils::SQLite3 & conn) {
    // check if table 'config' exists; if not, assume an empty database and create the tables
    libdnf::utils::SQLite3::Statement query_table_config_exists(conn, SQL_TABLE_CONFIG_EXISTS);
    if (query_table_config_exists.step() != libdnf::utils::SQLite3::Statement::StepResult::ROW) {
        conn.exec(SQL_CREATE_TABLES);
    }

    std::string schema_version;
    libdnf::utils::SQLite3::Statement query_get_schema_version(conn, SQL_GET_SCHEMA_VERSION);
    if (query_get_schema_version.step() == libdnf::utils::SQLite3::Statement::StepResult::ROW) {
        schema_version = query_get_schema_version.get<std::string>(0);
    } else {
        throw std::runtime_error("Unable to get 'version' from table 'config'");
    }

    // TODO(dmach): migrations
}


std::unique_ptr<libdnf::utils::SQLite3> transaction_db_connect(libdnf::Base & base) {
    std::string db_path;

    // use db_path based on persistdir
    auto & config = base.get_config();
    config.installroot().lock("installroot locked by transaction_db_connect");
    std::filesystem::path path(config.installroot().get_value());
    std::filesystem::path persistdir(config.persistdir().get_value());
    path /= persistdir.relative_path();
    std::filesystem::create_directories(path);
    path /= "history.sqlite";
    db_path = path.native();

    auto conn = std::make_unique<libdnf::utils::SQLite3>(db_path);
    transaction_db_create(*conn);
    return conn;
}


}  // namespace libdnf::transaction
