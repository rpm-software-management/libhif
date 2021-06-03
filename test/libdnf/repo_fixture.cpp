// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "repo_fixture.hpp"

#include <filesystem>
#include <map>


// Static map (class_name -> cache_dir) that allows re-using cache dirs among test cases in a class.
// Prevents creating solv files over and over again.
static std::map<std::string, std::unique_ptr<libdnf::utils::TempDir>> cache_dirs;


void RepoFixture::add_repo_repomd(const std::string & repoid) {
    auto repo = repo_sack->new_repo(repoid);

    // Sets the repo baseurl
    std::filesystem::path repo_path = PROJECT_SOURCE_DIR "/test/data/repos-repomd";
    repo_path /= repoid;
    repo->get_config().baseurl().set(libdnf::Option::Priority::RUNTIME, "file://" + repo_path.native());

    // Loads repository into rpm::Repo.
    repo->load();

    // Loads rpm::Repo into rpm::PackageSack
    sack->load_repo(*repo.get(),
        libdnf::rpm::PackageSack::LoadRepoFlags::USE_FILELISTS |
        libdnf::rpm::PackageSack::LoadRepoFlags::USE_OTHER |
        libdnf::rpm::PackageSack::LoadRepoFlags::USE_PRESTO |
        libdnf::rpm::PackageSack::LoadRepoFlags::USE_UPDATEINFO
    );
}


void RepoFixture::add_repo_rpm(const std::string & repoid) {
    auto repo = repo_sack->new_repo(repoid);

    // Sets the repo baseurl
    std::filesystem::path repo_path = PROJECT_BINARY_DIR "/test/data/repos-rpm";
    repo_path /= repoid;
    repo->get_config().baseurl().set(libdnf::Option::Priority::RUNTIME, "file://" + repo_path.native());

    // Loads repository into rpm::Repo.
    repo->load();

    // Loads rpm::Repo into rpm::PackageSack
    sack->load_repo(*repo.get(),
        libdnf::rpm::PackageSack::LoadRepoFlags::USE_FILELISTS |
        libdnf::rpm::PackageSack::LoadRepoFlags::USE_OTHER |
        libdnf::rpm::PackageSack::LoadRepoFlags::USE_PRESTO |
        libdnf::rpm::PackageSack::LoadRepoFlags::USE_UPDATEINFO
    );
}


void RepoFixture::add_repo_solv(const std::string & repoid) {
    std::filesystem::path repo_path = PROJECT_SOURCE_DIR "/test/data/repos-solv";
    repo_path /= repoid + ".repo";
    repo_sack->new_repo_from_libsolv_testcase(repoid.c_str(), repo_path.native());
}


void RepoFixture::setUp() {
    TestCaseFixture::setUp();

    temp = std::make_unique<libdnf::utils::TempDir>(
        "libdnf_unittest_",
        std::vector<std::string>{"installroot"}
    );
    base = std::make_unique<libdnf::Base>();

    // set installroot to a temp directory
    base->get_config().installroot().set(libdnf::Option::Priority::RUNTIME, temp->get_path() / "installroot");

    // use the shared cache dir (see cache_dirs comment for more details)
    auto class_name = typeid(*this).name();
    auto it = cache_dirs.find(class_name);
    if (it == cache_dirs.end()) {
        cache_dirs.insert({class_name, std::make_unique<libdnf::utils::TempDir>("libdnf_unittest_")});
    }
    base->get_config().cachedir().set(libdnf::Option::Priority::RUNTIME, cache_dirs.at(class_name)->get_path());

    repo_sack = base->get_rpm_repo_sack();
    sack = base->get_rpm_package_sack();
}

void RepoFixture::dump_debugdata() {
    sack->dump_debugdata("debugdata");
}
