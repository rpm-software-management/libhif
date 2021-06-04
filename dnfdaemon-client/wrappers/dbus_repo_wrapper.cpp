// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "dbus_repo_wrapper.hpp"

namespace dnfdaemon::client {

std::vector<std::pair<std::string, std::string>> DbusRepoWrapper::get_distro_tags() const {
    // sdbus::Variant cannot handle vector<pair<string,string>> so values are
    // serialized to vector<string>.
    // convert [tag1, val1, tag2, val2,...] back to [(tag1, val1), (tag2, val2),...]
    std::vector<std::pair<std::string, std::string>> dt{};
    std::vector<std::string> tags_raw = rawdata.at("distro_tags");
    if (!tags_raw.empty()) {
        for (size_t i = 0; i < (tags_raw.size() - 1); i += 2) {
            dt.emplace_back(tags_raw[i], tags_raw[i + 1]);
        }
    }
    return dt;
}

}  // namespace dnfdaemon::client
