// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "utils.hpp"


std::vector<std::string> to_vector_string(const libdnf::rpm::ReldepList & rdl) {
    std::vector<std::string> res;
    for (int i = 0; i < rdl.size(); ++i) {
        res.emplace_back(rdl.get(i).to_string());
    }
    return res;
}


std::vector<std::string> to_vector_string(const libdnf::rpm::PackageSet & pset) {
    std::vector<std::string> res;
    for (auto pkg : pset) {
        res.emplace_back(pkg.get_full_nevra());
    }
    return res;
}


std::vector<std::string> to_vector_string(const std::vector<libdnf::rpm::Package> & pkg_list) {
    std::vector<std::string> result;
    for (auto & pkg : pkg_list) {
        result.emplace_back(pkg.get_full_nevra());
    }
    return result;
}


std::vector<std::string> to_vector_name_string(const libdnf::advisory::AdvisoryQuery & advisory_query) {
    std::vector<std::string> result;
    for (auto & adv : advisory_query.get_advisories()) {
        result.emplace_back(adv.get_name());
    }
    return result;
}
