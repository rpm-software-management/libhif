// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "package_private.hpp"

#include <filesystem>

namespace libdnf::rpm::solv {

const char * get_full_nevra(Pool * pool, Id package_id) {
    Solvable * solvable = get_solvable(pool, package_id);
    const char * name = pool_id2str(pool, solvable->name);
    const char * evr = pool_id2str(pool, solvable->evr);
    const char * arch = pool_id2str(pool, solvable->arch);
    bool present_epoch = false;

    for (const char * e = evr + 1; *e != '-' && *e != '\0'; ++e) {
        if (*e == ':') {
            present_epoch = true;
            break;
        }
    }
    char * output_string;
    int extra_epoch_length = 0;
    int name_length = static_cast<int>(strlen(name));
    int evr_length = static_cast<int>(strlen(evr));
    int arch_length = static_cast<int>(strlen(arch));
    if (!present_epoch) {
        extra_epoch_length = 2;
    }

    output_string = pool_alloctmpspace(pool, name_length + evr_length + extra_epoch_length + arch_length + 3);

    strcpy(output_string, name);

    output_string[name_length++] = '-';

    if (extra_epoch_length == 2) {
        output_string[name_length++] = '0';
        output_string[name_length++] = ':';
        output_string[name_length] = '\0';
    }

    if (evr_length) {
        strcpy(output_string + name_length, evr);
        name_length += evr_length;
    }

    if (arch_length) {
        output_string[name_length++] = '.';
        strcpy(output_string + name_length, arch);
    }
    return output_string;
}

//TODO(jrohel): What about local repositories? The original code in DNF4 uses baseurl+get_location(pool, package_id).
std::string get_package_path(Pool * pool, Id package_id) {
    auto solvable = get_solvable(pool, package_id);
    if (auto repo = static_cast<repo::Repo *>(solvable->repo->appdata)) {
        auto dir = std::filesystem::path(repo->get_cachedir()) / "packages";
        return dir / std::filesystem::path(get_location(pool, package_id)).filename();
    } else {
        return "";
    }
}

}  // namespace libdnf::rpm::solv
