// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#include "test_goal.hpp"

#include "test/libdnf/utils.hpp"

#include "libdnf/base/goal.hpp"

#include <libdnf/rpm/package_query.hpp>


CPPUNIT_TEST_SUITE_REGISTRATION(BaseGoalTest);

void BaseGoalTest::setUp() {
    RepoFixture::setUp();
    add_repo_repomd("repomd-repo1");
}

void BaseGoalTest::test_install() {
    libdnf::Goal goal(base.get());
    goal.add_rpm_install("pkg");
    goal.resolve(false);
    auto install_set = goal.list_rpm_installs();
    auto reinstall_set = goal.list_rpm_reinstalls();
    auto upgrade_set = goal.list_rpm_upgrades();
    auto downgrade_set = goal.list_rpm_downgrades();
    auto remove_set = goal.list_rpm_removes();
    auto obsoleted_set = goal.list_rpm_obsoleted();
    CPPUNIT_ASSERT_EQUAL(1lu, install_set.size());
    CPPUNIT_ASSERT_EQUAL(std::string("pkg-0:1.2-3.x86_64"), install_set[0].get_full_nevra());
    CPPUNIT_ASSERT_EQUAL(0lu, reinstall_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, upgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, downgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, remove_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, obsoleted_set.size());
}

void BaseGoalTest::test_install_not_available() {
    libdnf::Goal goal(base.get());
    base->get_config().strict().set(libdnf::Option::Priority::RUNTIME, false);
    base->get_config().best().set(libdnf::Option::Priority::RUNTIME, true);
    base->get_config().clean_requirements_on_remove().set(libdnf::Option::Priority::RUNTIME, true);
    goal.add_rpm_install("not_available");
    goal.resolve(false);
    auto install_set = goal.list_rpm_installs();
    auto reinstall_set = goal.list_rpm_reinstalls();
    auto upgrade_set = goal.list_rpm_upgrades();
    auto downgrade_set = goal.list_rpm_downgrades();
    auto remove_set = goal.list_rpm_removes();
    auto obsoleted_set = goal.list_rpm_obsoleted();
    CPPUNIT_ASSERT_EQUAL(0lu, install_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, reinstall_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, upgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, downgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, remove_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, obsoleted_set.size());

    auto & log = goal.get_resolve_log();
    CPPUNIT_ASSERT_EQUAL(1lu, log.size());
    auto & [action, problem, settings, spec, additional_data] = *log.begin();
    CPPUNIT_ASSERT_EQUAL(libdnf::Goal::Action::INSTALL, action);
    CPPUNIT_ASSERT_EQUAL(libdnf::GoalProblem::NOT_FOUND, problem);
    CPPUNIT_ASSERT_EQUAL(std::string("not_available"), spec);
    CPPUNIT_ASSERT_EQUAL(libdnf::GoalUsedSetting::USED_FALSE, settings.get_used_clean_requirements_on_remove());
    CPPUNIT_ASSERT_EQUAL(libdnf::GoalUsedSetting::USED_TRUE, settings.get_used_best());
    CPPUNIT_ASSERT_EQUAL(libdnf::GoalUsedSetting::USED_FALSE, settings.get_used_strict());
}

void BaseGoalTest::test_install_from_cmdline() {
    // this test covers an old dnf bug described in the following steps:
    // * specify a commandline package to install
    // * the specified package has the same NEVRA as an available package in a repo
    // * the package query uses NEVRA instead of id and is resolved into the available rather than the specified package
    // * -> an unexpected package is installed

    // add a repo with package 'one-0:1-1.noarch'
    add_repo_rpm("rpm-repo1");

    // add 'one-0:1-1.noarch' package from the command-line
    std::filesystem::path rpm_path = PROJECT_BINARY_DIR "/test/data/repos-rpm/rpm-repo1/one-1-1.noarch.rpm";
    auto cmdline_pkg = sack->add_cmdline_package(rpm_path, false);

    // install the command-line package
    libdnf::Goal goal(base.get());
    goal.add_rpm_install(cmdline_pkg);

    // resolve the goal and read results
    goal.resolve(false);
    auto install_set = goal.list_rpm_installs();
    auto reinstall_set = goal.list_rpm_reinstalls();
    auto upgrade_set = goal.list_rpm_upgrades();
    auto downgrade_set = goal.list_rpm_downgrades();
    auto remove_set = goal.list_rpm_removes();
    auto obsoleted_set = goal.list_rpm_obsoleted();

    // check if we're getting an expected NEVRA
    std::vector<std::string> expected = {"one-0:1-1.noarch"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_string(install_set));

    // also check that the installed package is identical to the command-line package
    CPPUNIT_ASSERT_EQUAL(cmdline_pkg, install_set[0]);

    CPPUNIT_ASSERT_EQUAL(0lu, reinstall_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, upgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, downgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, remove_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, obsoleted_set.size());
}

void BaseGoalTest::test_install_multilib_all() {
    add_repo_solv("solv-multilib");
    base->get_config().multilib_policy().set(libdnf::Option::Priority::RUNTIME, "all");

    // install the command-line package
    libdnf::Goal goal(base.get());
    goal.add_rpm_install("multilib");

    // resolve the goal and read results
    goal.resolve(false);
    auto install_set = goal.list_rpm_installs();
    auto reinstall_set = goal.list_rpm_reinstalls();
    auto upgrade_set = goal.list_rpm_upgrades();
    auto downgrade_set = goal.list_rpm_downgrades();
    auto remove_set = goal.list_rpm_removes();
    auto obsoleted_set = goal.list_rpm_obsoleted();

    CPPUNIT_ASSERT_EQUAL(2lu, install_set.size());
    // check if we're getting an expected NEVRA
    std::vector<std::string> expected = {"multilib-0:1.2-4.x86_64", "multilib-0:1.2-4.i686"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_string(install_set));
    CPPUNIT_ASSERT_EQUAL(0lu, reinstall_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, upgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, downgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, remove_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, obsoleted_set.size());
}

void BaseGoalTest::test_reinstall() {
    std::filesystem::path rpm_path = PROJECT_BINARY_DIR "/test/data/cmdline-rpms/cmdline-1.2-3.noarch.rpm";

    // add the package to the @System repo so it appears as installed
    sack->add_system_package(rpm_path, false, false);

    // also add it to the @Commandline repo to make it available for reinstall
    sack->add_cmdline_package(rpm_path, false);

    libdnf::Goal goal(base.get());
    goal.add_rpm_reinstall("cmdline");

    goal.resolve(false);
    auto install_set = goal.list_rpm_installs();
    auto reinstall_set = goal.list_rpm_reinstalls();
    auto upgrade_set = goal.list_rpm_upgrades();
    auto downgrade_set = goal.list_rpm_downgrades();
    auto remove_set = goal.list_rpm_removes();
    auto obsoleted_set = goal.list_rpm_obsoleted();

    // the package is installed already, install_set is empty
    CPPUNIT_ASSERT_EQUAL(0lu, install_set.size());
    CPPUNIT_ASSERT_EQUAL(1lu, reinstall_set.size());
    CPPUNIT_ASSERT_EQUAL(reinstall_set[0].get_full_nevra(), std::string("cmdline-0:1.2-3.noarch"));
    CPPUNIT_ASSERT_EQUAL(0lu, upgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, downgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, remove_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, obsoleted_set.size());
}

void BaseGoalTest::test_remove() {
    std::filesystem::path rpm_path = PROJECT_BINARY_DIR "/test/data/cmdline-rpms/cmdline-1.2-3.noarch.rpm";
    sack->add_system_package(rpm_path, false, false);
    libdnf::Goal goal(base.get());
    goal.add_rpm_remove("cmdline");
    goal.resolve(false);
    auto install_set = goal.list_rpm_installs();
    auto reinstall_set = goal.list_rpm_reinstalls();
    auto upgrade_set = goal.list_rpm_upgrades();
    auto downgrade_set = goal.list_rpm_downgrades();
    auto remove_set = goal.list_rpm_removes();
    auto obsoleted_set = goal.list_rpm_obsoleted();
    CPPUNIT_ASSERT_EQUAL(0lu, install_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, reinstall_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, upgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, downgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(1lu, remove_set.size());
    CPPUNIT_ASSERT_EQUAL(remove_set[0].get_full_nevra(), std::string("cmdline-0:1.2-3.noarch"));
    CPPUNIT_ASSERT_EQUAL(0lu, obsoleted_set.size());
}

void BaseGoalTest::test_remove_not_installed() {
    base->get_config().clean_requirements_on_remove().set(libdnf::Option::Priority::RUNTIME, true);
    std::filesystem::path rpm_path = PROJECT_BINARY_DIR "/test/data/cmdline-rpms/cmdline-1.2-3.noarch.rpm";
    sack->add_system_package(rpm_path, false, false);
    libdnf::Goal goal(base.get());
    goal.add_rpm_remove("not_installed");
    goal.resolve(false);
    auto install_set = goal.list_rpm_installs();
    auto reinstall_set = goal.list_rpm_reinstalls();
    auto upgrade_set = goal.list_rpm_upgrades();
    auto downgrade_set = goal.list_rpm_downgrades();
    auto remove_set = goal.list_rpm_removes();
    auto obsoleted_set = goal.list_rpm_obsoleted();
    CPPUNIT_ASSERT_EQUAL(0lu, install_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, reinstall_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, upgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, downgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, remove_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, obsoleted_set.size());

    auto & log = goal.get_resolve_log();
    CPPUNIT_ASSERT_EQUAL(1lu, log.size());
    auto & [action, problem, settings, spec, additional_data] = *log.begin();
    CPPUNIT_ASSERT_EQUAL(libdnf::Goal::Action::REMOVE, action);
    CPPUNIT_ASSERT_EQUAL(libdnf::GoalProblem::NOT_FOUND, problem);
    CPPUNIT_ASSERT_EQUAL(std::string("not_installed"), spec);
    CPPUNIT_ASSERT_EQUAL(libdnf::GoalUsedSetting::USED_TRUE, settings.get_used_clean_requirements_on_remove());
    CPPUNIT_ASSERT_EQUAL(libdnf::GoalUsedSetting::UNUSED, settings.get_used_best());
    CPPUNIT_ASSERT_EQUAL(libdnf::GoalUsedSetting::UNUSED, settings.get_used_strict());
}

void BaseGoalTest::test_install_installed_pkg() {
    std::filesystem::path rpm_path = PROJECT_BINARY_DIR "/test/data/cmdline-rpms/cmdline-1.2-3.noarch.rpm";

    // add the package to the @System repo so it appears installed
    sack->add_system_package(rpm_path, false, false);

    // also add it to the @Commandline repo to make it available for install
    sack->add_cmdline_package(rpm_path, false);

    libdnf::rpm::PackageQuery query(base->get_rpm_package_sack());
    query.filter_available().filter_nevra({"cmdline-0:1.2-3.noarch"});

    std::vector<std::string> expected = {"cmdline-0:1.2-3.noarch"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_string(query));

    libdnf::Goal goal(base.get());
    goal.add_rpm_install(query);

    goal.resolve(false);
    auto install_set = goal.list_rpm_installs();
    auto reinstall_set = goal.list_rpm_reinstalls();
    auto upgrade_set = goal.list_rpm_upgrades();
    auto downgrade_set = goal.list_rpm_downgrades();
    auto remove_set = goal.list_rpm_removes();
    auto obsoleted_set = goal.list_rpm_obsoleted();

    // the package is installed already, install_set is empty
    CPPUNIT_ASSERT_EQUAL(0lu, install_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, reinstall_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, upgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, downgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, remove_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, obsoleted_set.size());
}

void BaseGoalTest::test_upgrade() {
    std::filesystem::path rpm_path = PROJECT_BINARY_DIR "/test/data/cmdline-rpms/cmdline-1.2-3.noarch.rpm";

    // add the package to the @System repo so it appears installed
    sack->add_system_package(rpm_path, false, false);

    add_repo_solv("solv-upgrade");

    libdnf::rpm::PackageQuery query(base->get_rpm_package_sack());

    libdnf::Goal goal(base.get());
    goal.add_rpm_upgrade("cmdline");

    goal.resolve(false);
    auto install_set = goal.list_rpm_installs();
    auto reinstall_set = goal.list_rpm_reinstalls();
    auto upgrade_set = goal.list_rpm_upgrades();
    auto downgrade_set = goal.list_rpm_downgrades();
    auto remove_set = goal.list_rpm_removes();
    auto obsoleted_set = goal.list_rpm_obsoleted();

    // the package is installed already, install_set is empty
    CPPUNIT_ASSERT_EQUAL(0lu, install_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, reinstall_set.size());
    CPPUNIT_ASSERT_EQUAL(1lu, upgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(upgrade_set[0].get_full_nevra(), std::string("cmdline-0:1.2-4.noarch"));
    CPPUNIT_ASSERT_EQUAL(0lu, downgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, remove_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, obsoleted_set.size());
}

void BaseGoalTest::test_upgrade_not_available() {
    base->get_config().best().set(libdnf::Option::Priority::RUNTIME, true);
    base->get_config().clean_requirements_on_remove().set(libdnf::Option::Priority::RUNTIME, true);
    std::filesystem::path rpm_path = PROJECT_BINARY_DIR "/test/data/cmdline-rpms/cmdline-1.2-3.noarch.rpm";

    // add the package to the @System repo so it appears installed
    sack->add_system_package(rpm_path, false, false);

    add_repo_solv("solv-upgrade");

    libdnf::rpm::PackageQuery query(base->get_rpm_package_sack());

    libdnf::Goal goal(base.get());
    goal.add_rpm_upgrade("not_available");

    goal.resolve(false);
    auto install_set = goal.list_rpm_installs();
    auto reinstall_set = goal.list_rpm_reinstalls();
    auto upgrade_set = goal.list_rpm_upgrades();
    auto downgrade_set = goal.list_rpm_downgrades();
    auto remove_set = goal.list_rpm_removes();
    auto obsoleted_set = goal.list_rpm_obsoleted();

    // the package is installed already, install_set is empty
    CPPUNIT_ASSERT_EQUAL(0lu, install_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, reinstall_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, upgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, downgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, remove_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, obsoleted_set.size());

    auto & log = goal.get_resolve_log();
    CPPUNIT_ASSERT_EQUAL(1lu, log.size());
    auto & [action, problem, settings, spec, additional_data] = *log.begin();
    CPPUNIT_ASSERT_EQUAL(libdnf::Goal::Action::UPGRADE, action);
    CPPUNIT_ASSERT_EQUAL(libdnf::GoalProblem::NOT_FOUND, problem);
    CPPUNIT_ASSERT_EQUAL(std::string("not_available"), spec);
    CPPUNIT_ASSERT_EQUAL(libdnf::GoalUsedSetting::USED_FALSE, settings.get_used_clean_requirements_on_remove());
    CPPUNIT_ASSERT_EQUAL(libdnf::GoalUsedSetting::USED_TRUE, settings.get_used_best());
    CPPUNIT_ASSERT_EQUAL(libdnf::GoalUsedSetting::UNUSED, settings.get_used_strict());
}

void BaseGoalTest::test_upgrade_all() {
    std::filesystem::path rpm_path = PROJECT_BINARY_DIR "/test/data/cmdline-rpms/cmdline-1.2-3.noarch.rpm";

    // add the package to the @System repo so it appears installed
    sack->add_system_package(rpm_path, false, false);

    add_repo_solv("solv-upgrade");

    libdnf::rpm::PackageQuery query(base->get_rpm_package_sack());

    libdnf::Goal goal(base.get());
    goal.add_rpm_upgrade();

    goal.resolve(false);
    auto install_set = goal.list_rpm_installs();
    auto reinstall_set = goal.list_rpm_reinstalls();
    auto upgrade_set = goal.list_rpm_upgrades();
    auto downgrade_set = goal.list_rpm_downgrades();
    auto remove_set = goal.list_rpm_removes();
    auto obsoleted_set = goal.list_rpm_obsoleted();

    // the package is installed already, install_set is empty
    CPPUNIT_ASSERT_EQUAL(0lu, install_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, reinstall_set.size());
    CPPUNIT_ASSERT_EQUAL(1lu, upgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(upgrade_set[0].get_full_nevra(), std::string("cmdline-0:1.2-4.noarch"));
    CPPUNIT_ASSERT_EQUAL(0lu, downgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, remove_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, obsoleted_set.size());
}

void BaseGoalTest::test_downgrade() {
    std::filesystem::path rpm_path = PROJECT_BINARY_DIR "/test/data/cmdline-rpms/cmdline-1.2-3.noarch.rpm";

    // add the package to the @System repo so it appears installed
    sack->add_system_package(rpm_path, false, false);

    add_repo_solv("solv-downgrade");

    libdnf::rpm::PackageQuery query(base->get_rpm_package_sack());

    libdnf::Goal goal(base.get());
    goal.add_rpm_downgrade("cmdline");

    goal.resolve(false);
    auto install_set = goal.list_rpm_installs();
    auto reinstall_set = goal.list_rpm_reinstalls();
    auto upgrade_set = goal.list_rpm_upgrades();
    auto downgrade_set = goal.list_rpm_downgrades();
    auto remove_set = goal.list_rpm_removes();
    auto obsoleted_set = goal.list_rpm_obsoleted();

    // the package is installed already, install_set is empty
    CPPUNIT_ASSERT_EQUAL(0lu, install_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, reinstall_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, upgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(1lu, downgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(downgrade_set[0].get_full_nevra(), std::string("cmdline-0:1.2-1.noarch"));
    CPPUNIT_ASSERT_EQUAL(0lu, remove_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, obsoleted_set.size());
}

void BaseGoalTest::test_distrosync() {
    std::filesystem::path rpm_path = PROJECT_BINARY_DIR "/test/data/cmdline-rpms/cmdline-1.2-3.noarch.rpm";

    // add the package to the @System repo so it appears installed
    sack->add_system_package(rpm_path, false, false);

    add_repo_solv("solv-distrosync");

    libdnf::rpm::PackageQuery query(base->get_rpm_package_sack());

    libdnf::Goal goal(base.get());
    goal.add_rpm_distro_sync("cmdline");

    goal.resolve(false);
    auto install_set = goal.list_rpm_installs();
    auto reinstall_set = goal.list_rpm_reinstalls();
    auto upgrade_set = goal.list_rpm_upgrades();
    auto downgrade_set = goal.list_rpm_downgrades();
    auto remove_set = goal.list_rpm_removes();
    auto obsoleted_set = goal.list_rpm_obsoleted();

    // the package is installed already, install_set is empty
    CPPUNIT_ASSERT_EQUAL(0lu, install_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, reinstall_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, upgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(1lu, downgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(downgrade_set[0].get_full_nevra(), std::string("cmdline-0:1.2-1.noarch"));
    CPPUNIT_ASSERT_EQUAL(0lu, remove_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, obsoleted_set.size());
}

void BaseGoalTest::test_distrosync_all() {
    std::filesystem::path rpm_path = PROJECT_BINARY_DIR "/test/data/cmdline-rpms/cmdline-1.2-3.noarch.rpm";

    // add the package to the @System repo so it appears installed
    sack->add_system_package(rpm_path, false, false);

    add_repo_solv("solv-distrosync");

    libdnf::rpm::PackageQuery query(base->get_rpm_package_sack());

    libdnf::Goal goal(base.get());
    goal.add_rpm_distro_sync();

    goal.resolve(false);
    auto install_set = goal.list_rpm_installs();
    auto reinstall_set = goal.list_rpm_reinstalls();
    auto upgrade_set = goal.list_rpm_upgrades();
    auto downgrade_set = goal.list_rpm_downgrades();
    auto remove_set = goal.list_rpm_removes();
    auto obsoleted_set = goal.list_rpm_obsoleted();

    // the package is installed already, install_set is empty
    CPPUNIT_ASSERT_EQUAL(0lu, install_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, reinstall_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, upgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(1lu, downgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(downgrade_set[0].get_full_nevra(), std::string("cmdline-0:1.2-1.noarch"));
    CPPUNIT_ASSERT_EQUAL(0lu, remove_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, obsoleted_set.size());
}

void BaseGoalTest::test_install_or_reinstall() {
    std::filesystem::path rpm_path = PROJECT_BINARY_DIR "/test/data/cmdline-rpms/cmdline-1.2-3.noarch.rpm";

    // add the package to the @System repo so it appears installed
    sack->add_system_package(rpm_path, false, false);

    // also add it to the @Commandline repo to make it available for reinstall
    sack->add_cmdline_package(rpm_path, false);

    libdnf::Goal goal(base.get());
    libdnf::rpm::PackageQuery query(base->get_rpm_package_sack());
    query.filter_available().filter_nevra({"cmdline-0:1.2-3.noarch"});
    CPPUNIT_ASSERT_EQUAL(1lu, query.size());
    goal.add_rpm_install_or_reinstall(query);
    goal.resolve(false);
    auto install_set = goal.list_rpm_installs();
    auto reinstall_set = goal.list_rpm_reinstalls();
    auto upgrade_set = goal.list_rpm_upgrades();
    auto downgrade_set = goal.list_rpm_downgrades();
    auto remove_set = goal.list_rpm_removes();
    auto obsoleted_set = goal.list_rpm_obsoleted();
    CPPUNIT_ASSERT_EQUAL(0lu, install_set.size());
    CPPUNIT_ASSERT_EQUAL(1lu, reinstall_set.size());
    CPPUNIT_ASSERT_EQUAL(reinstall_set[0].get_full_nevra(), std::string("cmdline-0:1.2-3.noarch"));
    CPPUNIT_ASSERT_EQUAL(0lu, upgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, downgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, remove_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, obsoleted_set.size());
}
