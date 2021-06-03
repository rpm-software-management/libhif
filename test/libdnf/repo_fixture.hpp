// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef TEST_LIBDNF_REPO_FIXTURE_HPP
#define TEST_LIBDNF_REPO_FIXTURE_HPP

#include "testcase_fixture.hpp"

#include "libdnf/base/base.hpp"
#include "libdnf/rpm/package_sack.hpp"
#include "libdnf/repo/repo_sack.hpp"
#include "libdnf/utils/temp.hpp"


class RepoFixture : public TestCaseFixture {
public:
    void setUp() override;

    void dump_debugdata();

protected:
    void add_repo_repomd(const std::string & repoid);
    void add_repo_rpm(const std::string & repoid);
    void add_repo_solv(const std::string & repoid);

    std::unique_ptr<libdnf::Base> base;
    libdnf::repo::RepoSackWeakPtr repo_sack;
    libdnf::rpm::PackageSackWeakPtr sack;
    std::unique_ptr<libdnf::utils::TempDir> temp;
};


#endif  // TEST_LIBDNF_REPO_FIXTURE_HPP
