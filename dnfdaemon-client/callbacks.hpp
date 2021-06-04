// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef DNFDAEMON_CLIENT_CALLBACKS_HPP
#define DNFDAEMON_CLIENT_CALLBACKS_HPP

#include <libdnf-cli/progressbar/download_progress_bar.hpp>
#include <libdnf-cli/progressbar/multi_progress_bar.hpp>
#include <sdbus-c++/sdbus-c++.h>

#include <string>

namespace dnfdaemon::client {

class DbusCallback {
public:
    explicit DbusCallback(sdbus::IProxy * proxy, std::string session_object_path)
        : session_object_path(session_object_path)
        , proxy(proxy){};
    virtual ~DbusCallback() = default;

protected:
    std::string session_object_path;
    sdbus::IProxy * proxy;

    bool signature_valid(sdbus::Signal & signal);
};


class RepoCB final : public DbusCallback {
public:
    explicit RepoCB(sdbus::IProxy * proxy, std::string session_object_path);
    virtual ~RepoCB() = default;

    void start(sdbus::Signal & signal);
    void end(sdbus::Signal & signal);
    void progress(sdbus::Signal & signal);

private:
    libdnf::cli::progressbar::DownloadProgressBar progress_bar{-1, ""};
    std::size_t msg_lines{0};
    void print_progress_bar();
};


class PackageDownloadCB final : public DbusCallback {
public:
    explicit PackageDownloadCB(sdbus::IProxy * proxy, std::string session_object_path);
    virtual ~PackageDownloadCB() = default;

    void start(sdbus::Signal & signal);
    void end(sdbus::Signal & signal);
    void progress(sdbus::Signal & signal);
    void mirror_failure(sdbus::Signal & signal);

private:
    libdnf::cli::progressbar::MultiProgressBar multi_progress_bar;
    // map {package id: progressbar}
    std::map<int, std::unique_ptr<libdnf::cli::progressbar::DownloadProgressBar>> package_bars;

    libdnf::cli::progressbar::DownloadProgressBar * find_progress_bar(const int pkg_id) {
        if (package_bars.find(pkg_id) != package_bars.end()) {
            return package_bars.at(pkg_id).get();
        } else {
            return nullptr;
        }
    }
};


class TransactionCB final : public DbusCallback {
public:
    explicit TransactionCB(sdbus::IProxy * proxy, std::string session_object_path);
    virtual ~TransactionCB() = default;

    void verify_start(sdbus::Signal & signal);
    void verify_end(sdbus::Signal & signal);
    void verify_progress(sdbus::Signal & signal);

    void transaction_start(sdbus::Signal & signal);
    void transaction_end(sdbus::Signal & signal);
    void transaction_progress(sdbus::Signal & signal);

    void action_start(sdbus::Signal & signal);
    void action_end(sdbus::Signal & signal);
    void action_progress(sdbus::Signal & signal);

    void script_start(sdbus::Signal & signal);
    void script_stop(sdbus::Signal & signal);
    void script_error(sdbus::Signal & signal);

    void unpack_error(sdbus::Signal & signal);

    void finished(sdbus::Signal & signal);

private:
    libdnf::cli::progressbar::MultiProgressBar multi_progress_bar;
    libdnf::cli::progressbar::DownloadProgressBar * active_progress_bar{nullptr};
    std::vector<std::unique_ptr<libdnf::cli::progressbar::DownloadProgressBar>> progress_bars;

    void new_progress_bar(uint64_t total, const std::string & description);
};

}  // namespace dnfdaemon::client

#endif
