// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_UTILS_WEAK_PTR_HPP
#define LIBDNF_UTILS_WEAK_PTR_HPP

#include "exception.hpp"

#include <unordered_set>

namespace libdnf {

template <typename TPtr, bool ptr_owner>
struct WeakPtr;


/// WeakPtrGuard is a resource guard. WeakPtr instances register itself to the resource guard.
/// And the resource guard invalidates the registered WeakPtrs when the resource is unusable
/// (eg. its dependecny was released).
template <typename TPtr, bool weak_ptr_is_owner>
struct WeakPtrGuard {
public:
    using TWeakPtr = WeakPtr<TPtr, weak_ptr_is_owner>;

    WeakPtrGuard() = default;
    WeakPtrGuard(const WeakPtrGuard &) = delete;
    WeakPtrGuard(WeakPtrGuard && src) noexcept : registered_weak_ptrs(std::move(src.registered_weak_ptrs)) {
        for (auto it : registered_weak_ptrs) {
            it->guard = this;
        }
    }
    ~WeakPtrGuard() { clear(); }

    WeakPtrGuard & operator=(const WeakPtrGuard & src) = delete;
    WeakPtrGuard & operator=(WeakPtrGuard && src) noexcept {
        registered_weak_ptrs = std::move(src.registered_weak_ptrs);
        for (auto it : registered_weak_ptrs) {
            it->guard = this;
        }
    }

    /// Returns true if the guard is empty, false otherwise.
    bool empty() const noexcept { return registered_weak_ptrs.empty(); }

    /// Returns the number of registered weak pointers.
    size_t size() const noexcept { return registered_weak_ptrs.size(); }

    /// Deregisters and invalidates all registered weak pointers. After this call, size() returns zero.
    void clear() noexcept {
        for (auto it : registered_weak_ptrs) {
            it->invalidate_guard();
        }
        registered_weak_ptrs.clear();
    }

private:
    friend TWeakPtr;
    void register_ptr(TWeakPtr * weak_ptr) { registered_weak_ptrs.insert(weak_ptr); }
    void unregister_ptr(TWeakPtr * weak_ptr) noexcept { registered_weak_ptrs.erase(weak_ptr); }
    std::unordered_set<TWeakPtr *> registered_weak_ptrs;
};


/// WeakPtr is a "smart" pointer. It contains a pointer to resource and to guard of resource.
/// WeakPtr pointer can be owner of the resource. However, the resource itself may depend on another resource.
/// WeakPtr registers/unregisters itself at the guard of resource. And the resource guard invalidates
/// the registered WeakPtrs when the resource is unusable (eg. its dependecny was released).
// TODO(jrohel): We want thread-safety. Locking isn't for free. Idea is locking in more upper level. Future.
template <typename TPtr, bool ptr_owner>
struct WeakPtr {
public:
    using TWeakPtrGuard = WeakPtrGuard<TPtr, ptr_owner>;

    /// Exception generated when the managed object is not valid.
    class InvalidPtr : public InvalidPointer {
    public:
        using InvalidPointer::InvalidPointer;
        const char * get_domain_name() const noexcept override { return "libdnf::WeakPtr"; }
        const char * get_name() const noexcept override { return "InvalidPtr"; }
        const char * get_description() const noexcept override { return "Invalid pointer"; }
    };

    WeakPtr() : ptr(nullptr), guard(nullptr) { }

    WeakPtr(TPtr * ptr, TWeakPtrGuard * guard) : ptr(ptr), guard(guard) {
        if (ptr && !guard) {
            throw InvalidPtr("Creating a non-null WeakPtr without a guard is not allowed");
        }
        guard->register_ptr(this);
    }

    // TODO(jrohel): Want we to allow copying invalid WeakPtr?
    WeakPtr(const WeakPtr & src) : guard(src.guard) {
        if constexpr (ptr_owner) {
            ptr = src.ptr ? new TPtr(*src.ptr) : nullptr;
        } else {
            ptr = src.ptr;
        }
        if (is_valid()) {
            guard->register_ptr(this);
        }
    }

    // TODO(jrohel): Want we to allow moving invalid WeakPtr?
    template <typename T = TPtr, typename std::enable_if<sizeof(T) && ptr_owner, int>::type = 0>
    WeakPtr(WeakPtr && src) : guard(src.guard) {
        if (src.is_valid()) {
            src.guard->unregister_ptr(&src);
        }
        src.guard = nullptr;
        ptr = src.ptr;
        src.ptr = nullptr;
        if (is_valid()) {
            guard->register_ptr(this);
        }
    }

    ~WeakPtr() {
        if (is_valid()) {
            guard->unregister_ptr(this);
        }
        if constexpr (ptr_owner) {
            delete ptr;
        }
    }

    // TODO(jrohel): Want we to allow copying invalid WeakPtr?
    WeakPtr & operator=(const WeakPtr & src) {
        if (guard == src.guard) {
            if (this == &src) {
                return *this;
            }
            if constexpr (ptr_owner) {
                delete ptr;
                ptr = src.ptr ? new TPtr(*src.ptr) : nullptr;
            } else {
                ptr = src.ptr;
            }
        } else {
            if (is_valid()) {
                guard->unregister_ptr(this);
            }
            guard = src.guard;
            if constexpr (ptr_owner) {
                delete ptr;
                ptr = src.ptr ? new TPtr(*src.ptr) : nullptr;
            } else {
                ptr = src.ptr;
            }
            if (is_valid()) {
                guard->register_ptr(this);
            }
        }
        return *this;
    }

    // TODO(jrohel): Want we to allow moving invalid WeakPtr?
    template <typename T = TPtr, typename std::enable_if<sizeof(T) && ptr_owner, int>::type = 0>
    WeakPtr & operator=(WeakPtr && src) {
        if (guard == src.guard) {
            if (this == &src) {
                return *this;
            }
            if (src.is_valid()) {
                src.guard->unregister_ptr(&src);
            }
            src.guard = nullptr;
            delete ptr;
            ptr = src.ptr;
            src.ptr = nullptr;
        } else {
            if (is_valid()) {
                guard->unregister_ptr(this);
            }
            if (src.is_valid()) {
                src.guard->unregister_ptr(&src);
            }
            guard = src.guard;
            src.guard = nullptr;
            delete ptr;
            ptr = src.ptr;
            src.ptr = nullptr;
            if (is_valid()) {
                guard->register_ptr(this);
            }
        }
        return *this;
    }

    /// Provides access to the managed object. Generates exception if object is not valid.
    TPtr * operator->() const {
        check();
        return ptr;
    }

    /// Returns a pointer to the managed object. Generates exception if object is not valid.
    TPtr * get() const {
        check();
        return ptr;
    }

    /// Checks if managed object is valid.
    bool is_valid() const noexcept { return guard; }

    /// Checks if the other WeakPtr instance has the same WeakPtrGuard.
    bool has_same_guard(const WeakPtr & other) const noexcept { return guard == other.guard; }

    TPtr & operator*() const { return *get(); }
    bool operator==(const WeakPtr & other) const { return ptr == other.ptr; }
    bool operator!=(const WeakPtr & other) const { return ptr != other.ptr; }
    bool operator<(const WeakPtr & other) const { return ptr < other.ptr; }
    bool operator>(const WeakPtr & other) const { return ptr > other.ptr; }
    bool operator<=(const WeakPtr & other) const { return ptr <= other.ptr; }
    bool operator>=(const WeakPtr & other) const { return ptr >= other.ptr; }

private:
    friend TWeakPtrGuard;
    void invalidate_guard() noexcept { guard = nullptr; }
    void check() const {
        if (!is_valid()) {
            throw InvalidPtr("Data guard is invalid");
        }
    }

    TPtr * ptr;
    TWeakPtrGuard * guard;
};

}  // namespace libdnf

#endif
