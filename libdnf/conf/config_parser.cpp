// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "libdnf/conf/config_parser.hpp"

#include "libdnf/utils/iniparser.hpp"

#include <algorithm>
#include <fstream>

namespace libdnf {

static void read(ConfigParser & cfg_parser, IniParser & parser) {
    IniParser::ItemType readed_type;
    while ((readed_type = parser.next()) != IniParser::ItemType::END_OF_INPUT) {
        auto section = parser.get_section();
        if (readed_type == IniParser::ItemType::SECTION) {
            cfg_parser.add_section(std::move(section), std::move(parser.get_raw_item()));
        } else if (readed_type == IniParser::ItemType::KEY_VAL) {
            cfg_parser.set_value(
                section, std::move(parser.get_key()), std::move(parser.get_value()), std::move(parser.get_raw_item()));
        } else if (readed_type == IniParser::ItemType::COMMENT_LINE || readed_type == IniParser::ItemType::EMPTY_LINE) {
            if (section.empty()) {
                cfg_parser.get_header() += parser.get_raw_item();
            } else {
                cfg_parser.add_comment_line(section, std::move(parser.get_raw_item()));
            }
        }
    }
}

void ConfigParser::read(const std::string & file_path) {
    IniParser parser(file_path);
    ::libdnf::read(*this, parser);
}

void ConfigParser::read(std::unique_ptr<std::istream> && input_stream) {
    IniParser parser(std::move(input_stream));
    ::libdnf::read(*this, parser);
}

static std::string create_raw_item(const std::string & value, const std::string & old_raw_item) {
    auto eql_pos = old_raw_item.find('=');
    if (eql_pos == std::string::npos) {
        return "";
    }
    auto value_pos = old_raw_item.find_first_not_of(" \t", eql_pos + 1);
    auto key_and_delim_length = value_pos != std::string::npos ? value_pos : old_raw_item.length();
    return old_raw_item.substr(0, key_and_delim_length) + value + '\n';
}

void ConfigParser::set_value(const std::string & section, const std::string & key, const std::string & value) {
    auto raw_iter = raw_items.find(section + ']' + key);
    auto raw = create_raw_item(value, raw_iter != raw_items.end() ? raw_iter->second : "");
    set_value(section, key, value, raw);
}

void ConfigParser::set_value(const std::string & section, std::string && key, std::string && value) {
    auto raw_iter = raw_items.find(section + ']' + key);
    auto raw = create_raw_item(value, raw_iter != raw_items.end() ? raw_iter->second : "");
    set_value(section, std::move(key), std::move(value), std::move(raw));
}

const std::string & ConfigParser::get_value(const std::string & section, const std::string & key) const {
    auto sect = data.find(section);
    if (sect == data.end()) {
        throw SectionNotFound(section);
    }
    auto key_val = sect->second.find(key);
    if (key_val == sect->second.end()) {
        throw OptionNotFound(key + " in section " + section);
    }
    return key_val->second;
}

static void write_key_vals(
    std::ostream & out,
    const std::string & section,
    const ConfigParser::Container::mapped_type & key_val_map,
    const std::map<std::string, std::string> & raw_items) {
    for (const auto & key_val : key_val_map) {
        auto first = key_val.first[0];
        if (first == '#' || first == ';') {
            out << key_val.second;
        } else {
            auto raw_item = raw_items.find(section + ']' + key_val.first);
            if (raw_item != raw_items.end()) {
                out << raw_item->second;
            } else {
                out << key_val.first << "=";
                for (const auto chr : key_val.second) {
                    out << chr;
                    if (chr == '\n') {
                        out << " ";
                    }
                }
                out << "\n";
            }
        }
    }
}

static void write_section(
    std::ostream & out,
    const std::string & section,
    const ConfigParser::Container::mapped_type & key_val_map,
    const std::map<std::string, std::string> & raw_items) {
    auto raw_item = raw_items.find(section);
    if (raw_item != raw_items.end()) {
        out << raw_item->second;
    } else {
        out << "[" << section << "]"
            << "\n";
    }
    write_key_vals(out, section, key_val_map, raw_items);
}

void ConfigParser::write(const std::string & file_path, bool append) const {
    std::ofstream ofs;
    ofs.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    ofs.open(file_path, append ? std::ofstream::app : std::ofstream::trunc);
    write(ofs);
}

void ConfigParser::write(const std::string & file_path, bool append, const std::string & section) const {
    auto sit = data.find(section);
    if (sit == data.end()) {
        throw SectionNotFound(section);
    }
    std::ofstream ofs;
    ofs.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    ofs.open(file_path, append ? std::ofstream::app : std::ofstream::trunc);
    write_section(ofs, sit->first, sit->second, raw_items);
}

void ConfigParser::write(std::ostream & output_stream) const {
    output_stream << header;
    for (const auto & section : data) {
        write_section(output_stream, section.first, section.second, raw_items);
    }
}

void ConfigParser::write(std::ostream & output_stream, const std::string & section) const {
    auto sit = data.find(section);
    if (sit == data.end()) {
        throw SectionNotFound(section);
    }
    write_section(output_stream, sit->first, sit->second, raw_items);
}

}  // namespace libdnf
