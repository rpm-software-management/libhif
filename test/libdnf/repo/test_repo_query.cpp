// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "test_repo_query.hpp"

#include "libdnf/base/base.hpp"
#include "libdnf/repo/repo_sack.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(RepoQueryTest);


void RepoQueryTest::setUp() {}


void RepoQueryTest::tearDown() {}


void RepoQueryTest::test_query_basics() {
    libdnf::Base base;
    libdnf::repo::RepoSack repo_sack(base);

    // Creates new repositories in the repo_sack
    auto repo1 = repo_sack.new_repo("repo1");
    auto repo2 = repo_sack.new_repo("repo2");
    auto repo1_updates = repo_sack.new_repo("repo1_updates");
    auto repo2_updates = repo_sack.new_repo("repo2_updates");

    // Tunes configuration of repositories
    repo1->enable();
    repo2->disable();
    repo1_updates->disable();
    repo2_updates->enable();
    repo1->get_config().baseurl().set(libdnf::Option::Priority::RUNTIME, "file:///path/to/repo1");
    repo2->get_config().baseurl().set(libdnf::Option::Priority::RUNTIME, "https://host/path/to/repo2");
    repo1_updates->get_config().baseurl().set(libdnf::Option::Priority::RUNTIME, "https://host/path/to/repo1_updates");
    repo2_updates->get_config().baseurl().set(libdnf::Option::Priority::RUNTIME, "https://host/path/to/repo2_updates");

    // Creates new query on the repo_sack
    auto repo_query = repo_sack.new_query();
    CPPUNIT_ASSERT_EQUAL(repo_query.size(), static_cast<size_t>(4));
    CPPUNIT_ASSERT((repo_query == libdnf::Set{repo1, repo2, repo1_updates, repo2_updates}));

    // Tests filter_enabled method
    repo_query.filter_enabled(true);
    CPPUNIT_ASSERT((repo_query == libdnf::Set{repo1, repo2_updates}));

    // Tests filter_id method
    auto repo_query1 = repo_sack.new_query().filter_id("*updates", libdnf::sack::QueryCmp::GLOB);
    CPPUNIT_ASSERT((repo_query1 == libdnf::Set{repo1_updates, repo2_updates}));

    // Tests filter_local method
    repo_query1 = repo_sack.new_query().filter_local(false);
    CPPUNIT_ASSERT((repo_query1 == libdnf::Set{repo2, repo1_updates, repo2_updates}));
}
