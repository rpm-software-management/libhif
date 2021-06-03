// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "temp.hpp"

#include <cstdlib>


namespace libdnf::utils {


TempDir::TempDir(const std::string & prefix) {
    // get path to the system temp directory
    auto temp = std::filesystem::temp_directory_path();

    // append /<prefix>XXXXXX as requested in mkdtemp(3)
    temp /= prefix + "XXXXXX";
    path = mkdtemp(const_cast<char *>(temp.native().c_str()));
}


TempDir::TempDir(const std::string & prefix, const std::vector<std::string> & subdirs)
    : TempDir(prefix) {
    for (auto & subdir : subdirs) {
        std::filesystem::create_directory(path / subdir);
    }
}


TempDir::~TempDir() {
    // remove the temp directory and all its content
    try {
        std::filesystem::remove_all(path);
    } catch (std::filesystem::filesystem_error &) {
        // catch an exception that shouldn't be raised in a destructor
        // we should consider logging or printing the exception
    }
}


}  // namespace libdnf::utils
