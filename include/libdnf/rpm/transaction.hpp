// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_RPM_TRANSACTION_HPP
#define LIBDNF_RPM_TRANSACTION_HPP

#include "libdnf/common/exception.hpp"
#include "libdnf/common/weak_ptr.hpp"
#include "libdnf/rpm/package.hpp"

#include <memory>

namespace libdnf {

class Base;

}  // namespace libdnf

namespace libdnf::rpm {

class Transaction;
class TransactionCB;
class RpmProblemSet;

/// Class represents one item in transaction set.
class TransactionItem {
public:
    explicit TransactionItem(Package pkg) : pkg(std::move(pkg)) {}
    const Package & get_pkg() const noexcept { return pkg; }
    Package & get_pkg() noexcept { return pkg; }

private:
    Package pkg;
};

/// Class for access RPM header
class RpmHeader {
public:
    std::string get_name() const;
    uint64_t get_epoch() const noexcept;
    std::string get_version() const;
    std::string get_release() const;
    std::string get_arch() const;
    std::string get_evr() const;
    std::string get_nevra() const;
    std::string get_full_nevra() const;
    unsigned int get_rpmdbid() const noexcept;

private:
    friend Transaction;
    explicit RpmHeader(void * hdr) : header(hdr) {}
    void * header;
};

class RpmProblem {
public:
    /// Enumerate transaction set problem types.
    // TODO(jrohel): check value with librpm (static_assert?)
    enum class Type {
        BADARCH,            // package ... is for a different architecture
        BADOS,              // package ... is for a different operating system
        PKG_INSTALLED,      // package ... is already installed
        BADRELOCATE,        // path ... is not relocatable for package ...
        REQUIRES,           // package ... has unsatisfied Requires: ...
        CONFLICT,           // package ... has unsatisfied Conflicts: ...
        NEW_FILE_CONFLICT,  // file ... conflicts between attempted installs of ...
        FILE_CONFLICT,      // file ... from install of ... conflicts with file from package ...
        OLDPACKAGE,         // package ... (which is newer than ...) is already installed
        DISKSPACE,          // installing package ... needs ... on the ... filesystem
        DISKNODES,          // installing package ... needs ... on the ... filesystem
        OBSOLETES,          // package ... is obsoleted by ...
        VERIFY              // package did not pass verification
    };

    ~RpmProblem();

    bool operator==(RpmProblem & other) const noexcept;
    bool operator!=(RpmProblem & other) const noexcept;

    /// Return package NEVR
    std::string get_pkg_nevr() const;

    /// Return related (e.g. through a dependency) package NEVR
    std::string get_alt_nevr() const;

    /// Return type of problem (dependency, diskpace etc)
    Type get_type() const;

    /// Return pointer to transaction item associated to the problem or nullptr.
    const TransactionItem * get_transaction_item() const;

    /// Return a generic data string from a problem
    std::string get_generic_string() const;

    /// Return disk requirement (needed disk space / number of inodes)
    /// depending on problem type. On problem types other than DISKSPACE
    /// and DISKNODES return value is undefined.
    uint64_t get_disk_need() const;

    /// Return formatted string representation of a problem.
    std::string to_string() const;

private:
    friend RpmProblemSet;
    class Impl;
    explicit RpmProblem(std::unique_ptr<Impl> && p_impl);
    std::unique_ptr<Impl> p_impl;
};

class RpmProblemSet {
public:
    class Iterator {
    public:
        // iterator traits
        using difference_type = int;
        using value_type = RpmProblem;
        using pointer = const RpmProblem *;
        using reference = const RpmProblem &;
        using iterator_category = std::forward_iterator_tag;

        Iterator();
        ~Iterator();

        Iterator & operator++();
        // TODO(jrohel): postfix operator ++, Bad iterator support in the librpm
        // iterator operator++(int);
        bool operator==(Iterator & other) const;
        bool operator!=(Iterator other) const { return !(*this == other); }
        RpmProblem operator*();

    private:
        friend RpmProblemSet;
        class Impl;
        explicit Iterator(std::unique_ptr<Impl> && p_impl);
        std::unique_ptr<Impl> p_impl;
    };

    Iterator begin();
    Iterator end();

    ~RpmProblemSet();

    /// Return number of problems in set.
    /// @return  number of problems
    int size() noexcept;

    bool empty() noexcept { return size() == 0; }

private:
    friend Transaction;
    class Impl;
    explicit RpmProblemSet(std::unique_ptr<Impl> && p_impl);
    std::unique_ptr<Impl> p_impl;
};


class Transaction {
public:
    class Exception : public RuntimeError {
    public:
        using RuntimeError::RuntimeError;
        const char * get_domain_name() const noexcept override { return "libdnf::rpm::Transaction"; }
        const char * get_name() const noexcept override { return "Exception"; }
        const char * get_description() const noexcept override { return "rpm::Transaction exception"; }
    };

    // TODO(jrohel): Define enums or flag setters/getters
    using rpmVSFlags = uint32_t;
    using rpmtransFlags = uint32_t;
    using rpm_tid_t = uint32_t;

    explicit Transaction(Base & base);
    Transaction(const Transaction &) = delete;
    Transaction(Transaction &&) = delete;
    ~Transaction();

    Transaction & operator=(const Transaction &) = delete;
    Transaction & operator=(Transaction &&) = delete;

    /// Set transaction root directory, i.e. path to chroot(2).
    /// @param root_dir  new transaction root directory (or NULL)
    /// @return  0 on success, -1 on error (invalid root directory)
    void set_root_dir(const char * root_dir);

    /// Get transaction root directory, i.e. path to chroot(2).
    /// @return  transaction root directory
    const char * get_root_dir() const;

    /// Get transaction flags, i.e. bits that control rpmtsRun().
    /// @return  transaction flags
    rpmtransFlags get_flags() const;

    /// Set transaction flags, i.e. bits that control rpmtsRun().
    /// @param flags  new transaction flags
    /// @return  previous transaction flags
    rpmtransFlags set_flags(rpmtransFlags flags);

    /// Get verify signatures flag(s).
    /// @return  verify signatures flags
    rpmVSFlags get_signature_verify_flags() const;

    /// Set verify signatures flag(s).
    /// @param verify_flags  new verify signatures flags
    /// @return         previous value
    rpmVSFlags set_signature_verify_flags(rpmVSFlags verify_flags);

    /// Get package verify flag(s).
    /// @return  verify signatures flags
    rpmVSFlags get_pkg_verify_flags() const;

    /// Set package verify flag(s).
    /// @param verify_flags  new package verify flags
    /// @return  old package verify flags
    rpmVSFlags set_pkg_verify_flags(rpmVSFlags verify_flags);

    /// Get enforced package verify level
    /// @return  package verify level
    int get_pkg_verify_level() const;

    /// Set enforced package verify level
    /// @param verify_level  new package verify level
    /// @return  old package verify level
    int set_pkg_verify_level(int verify_level);

    /// Get transaction id, i.e. transaction time stamp.
    /// @return  transaction id
    rpm_tid_t get_id() const;

    // Set transaction notify callback.
    void register_cb(TransactionCB * cb);

    /// Add package to be installed to transaction set.
    /// The transaction set is checked for duplicate package names.
    /// If found, the package with the "newest" EVR will be replaced.
    /// @param item  item to be installed
    void install(TransactionItem & item);

    /// Add package to be upgraded to transaction set.
    /// The transaction set is checked for duplicate package names.
    /// If found, the package with the "newest" EVR will be replaced.
    /// @param item  item to be upgraded
    void upgrade(TransactionItem & item);

    /// Add package to be reinstalled to transaction set.
    /// @param item  item to be reinstalled
    void reinstall(TransactionItem & item);

    /// Add package to be erased to transaction set.
    /// @param item  item to be erased
    void erase(TransactionItem & item);

    /// Perform a dependency check on the transaction set.
    /// After headers have been added to a transaction set,
    /// a dependency check can be performed to make sure that all package dependencies are satisfied.
    /// Any found problems can be examined by retrieving the problem set with rpmtsProblems().
    /// @return  true on dependencies are ok
    bool check();

    /// Process all package elements in a transaction set.
    /// Before calling run() be sure to have:
    ///
    ///    - setup the rpm root dir via set_root_dir().
    ///    - setup the rpm notify callback via register_cb().
    ///    - setup the rpm transaction flags via set_flags().
    ///
    /// Additionally, though not required you may want to:
    ///
    ///    - setup the rpm verify signature flags via set_signature_verify_flags().
    ///
    /// @return		0 on success, -1 on error, >0 with newProbs set
    int run();

    /// Get current transaction set problems.
    RpmProblemSet get_problems();

    /// Set transaction script file descriptor, i.e. stdout/stderr on scriptlet execution.
    /// The file descriptor is copied using dup().
    /// @param fd  new script file descriptor (or -1)
    void set_script_out_fd(int fd);

    /// Set transaction script file, i.e. stdout/stderr on scriptlet execution.
    /// The set_script_fd() can be used to pass file handle.
    /// @param file_path  new file path
    void set_script_out_file(const std::string & file_path);

private:
    class Impl;
    std::unique_ptr<Impl> p_impl;
};


// suppress "unused-parameter" warnings because TransactionCB is a virtual class
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"


/// Base class for Transaction callbacks
/// User implements Transaction callbacks by inheriting this class and overriding its methods.
class TransactionCB {
public:
    virtual ~TransactionCB() = default;

    virtual void install_progress(
        const TransactionItem * item, const RpmHeader & header, uint64_t amount, uint64_t total) {}
    virtual void install_start(const TransactionItem * item, const RpmHeader & header, uint64_t total) {}
    virtual void install_stop(const TransactionItem * item, const RpmHeader & header, uint64_t amount, uint64_t total) {
    }
    virtual void transaction_progress(uint64_t amount, uint64_t total) {}
    virtual void transaction_start(uint64_t total) {}
    virtual void transaction_stop(uint64_t total) {}
    virtual void uninstall_progress(
        const TransactionItem * item, const RpmHeader & header, uint64_t amount, uint64_t total) {}
    virtual void uninstall_start(const TransactionItem * item, const RpmHeader & header, uint64_t total) {}
    virtual void uninstall_stop(
        const TransactionItem * item, const RpmHeader & header, uint64_t amount, uint64_t total) {}
    virtual void unpack_error(const TransactionItem * item, const RpmHeader & header) {}
    virtual void cpio_error(const TransactionItem * item, const RpmHeader & header) {}
    virtual void script_error(
        const TransactionItem * item, const RpmHeader & header, uint64_t tag, uint64_t return_code) {}
    virtual void script_start(const TransactionItem * item, const RpmHeader & header, uint64_t tag) {}
    virtual void script_stop(
        const TransactionItem * item, const RpmHeader & header, uint64_t tag, uint64_t return_code) {}
    virtual void elem_progress(
        const TransactionItem * item, const RpmHeader & header, uint64_t amount, uint64_t total) {}
    virtual void verify_progress(uint64_t amount, uint64_t total) {}
    virtual void verify_start(uint64_t total) {}
    virtual void verify_stop(uint64_t total) {}
};


#pragma GCC diagnostic pop


}  // namespace libdnf::rpm

#endif  // LIBDNF_RPM_TRANSACTION_HPP
