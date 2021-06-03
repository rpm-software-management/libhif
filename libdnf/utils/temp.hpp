// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_UTILS_TEMP_HPP
#define LIBDNF_UTILS_TEMP_HPP


#include <filesystem>
#include <vector>
#include <string>


namespace libdnf::utils {


/// Object that creates and holds a temp directory.
/// The directory gets removed when the object is deleted.
class TempDir {
public:
    explicit TempDir(const std::string & prefix);
    // create a temp directory with specified subdirs
    explicit TempDir(const std::string & prefix, const std::vector<std::string> & subdirs);
    TempDir(const TempDir &) = delete;
    ~TempDir();
    const std::filesystem::path & get_path() const noexcept { return path; }

private:
    std::filesystem::path path;
};


}  // namespace libdnf::utils


#endif  // LIBDNF_UTILS_TEMP_HPP
