// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef DNFDAEMON_SERVER_CALLBACKS_HPP
#define DNFDAEMON_SERVER_CALLBACKS_HPP

#include <libdnf/repo/repo.hpp>
#include <libdnf/rpm/transaction.hpp>
#include <sdbus-c++/sdbus-c++.h>

#include <chrono>
#include <string>

class Session;

class DbusCallback {
public:
    explicit DbusCallback(Session & session);
    virtual ~DbusCallback() = default;

protected:
    static std::chrono::time_point<std::chrono::steady_clock> prev_print_time;
    Session & session;
    sdbus::IObject * dbus_object;

    virtual sdbus::Signal create_signal(std::string interface, std::string signal_name);
    static bool is_time_to_print() {
        auto now = std::chrono::steady_clock::now();
        auto delta = now - prev_print_time;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(delta).count();
        if (ms > 400) {
            prev_print_time = now;
            return true;
        }
        return false;
    }
};

class DbusPackageCB : public libdnf::repo::PackageTargetCB, public DbusCallback {
public:
    explicit DbusPackageCB(Session & session, const libdnf::rpm::Package & pkg);
    virtual ~DbusPackageCB() = default;

    int end(TransferStatus status, const char * msg) override;
    int progress(double total_to_download, double downloaded) override;
    int mirror_failure(const char * msg, const char * url) override;

private:
    int pkg_id;
    double total = 0;

    sdbus::Signal create_signal(std::string interface, std::string signal_name) override;
};

class DbusRepoCB : public libdnf::repo::RepoCB, public DbusCallback {
public:
    explicit DbusRepoCB(Session & session) : DbusCallback(session) {}
    virtual ~DbusRepoCB() = default;

    void start(const char * what) override;
    void end() override;
    int progress(double total_to_download, double downloaded) override;

    // TODO(mblaha): how to ask the user for confirmation?
    bool repokey_import(
        [[maybe_unused]] const std::string & id,
        [[maybe_unused]] const std::string & user_id,
        [[maybe_unused]] const std::string & fingerprint,
        [[maybe_unused]] const std::string & url,
        [[maybe_unused]] long int timestamp) override {
        return false;
    }

private:
    double total = 0;
};


class DbusTransactionCB : public libdnf::rpm::TransactionCB, public DbusCallback {
public:
    explicit DbusTransactionCB(Session & session) : DbusCallback(session) {}
    virtual ~DbusTransactionCB() = default;

    // transaction preparation
    void transaction_start(uint64_t total) override;
    void transaction_progress(uint64_t amount, uint64_t total) override;
    void transaction_stop(uint64_t total) override;

    // install a package
    void install_start(
        const libdnf::rpm::TransactionItem * item, const libdnf::rpm::RpmHeader & header, uint64_t) override;
    void install_progress(
        const libdnf::rpm::TransactionItem * item,
        const libdnf::rpm::RpmHeader & header,
        uint64_t amount,
        uint64_t total) override;
    void install_stop(
        const libdnf::rpm::TransactionItem * item,
        const libdnf::rpm::RpmHeader & header,
        uint64_t amount,
        uint64_t total) override;

    // uninstall a package (the same messages as for install are used)
    void uninstall_start(
        const libdnf::rpm::TransactionItem * item, const libdnf::rpm::RpmHeader & header, uint64_t total) override {
        install_start(item, header, total);
    }
    void uninstall_progress(
        const libdnf::rpm::TransactionItem * item,
        const libdnf::rpm::RpmHeader & header,
        uint64_t amount,
        uint64_t total) override {
        install_progress(item, header, amount, total);
    }
    void uninstall_stop(
        const libdnf::rpm::TransactionItem * item,
        const libdnf::rpm::RpmHeader & header,
        uint64_t amount,
        uint64_t total) override {
        install_stop(item, header, amount, total);
    }

    void script_start(
        const libdnf::rpm::TransactionItem * item, const libdnf::rpm::RpmHeader & header, uint64_t tag) override;
    void script_stop(
        const libdnf::rpm::TransactionItem * item,
        const libdnf::rpm::RpmHeader & header,
        uint64_t tag,
        uint64_t return_code) override;
    void script_error(
        const libdnf::rpm::TransactionItem * item,
        const libdnf::rpm::RpmHeader & header,
        uint64_t tag,
        uint64_t return_code) override;

    void elem_progress(
        const libdnf::rpm::TransactionItem * item,
        const libdnf::rpm::RpmHeader & header,
        uint64_t amount,
        uint64_t total) override;

    void verify_start(uint64_t total) override;
    void verify_progress(uint64_t amount, uint64_t total) override;
    void verify_stop(uint64_t total) override;

    void unpack_error(const libdnf::rpm::TransactionItem * item, const libdnf::rpm::RpmHeader & header) override;
    void cpio_error(
        const libdnf::rpm::TransactionItem * /*item*/, const libdnf::rpm::RpmHeader & /*header*/) override{};

    // whole rpm transaction is finished
    void finish();

private:
    sdbus::Signal create_signal_pkg(
        std::string interface, std::string signal_name, const libdnf::rpm::RpmHeader & header);
};

#endif
