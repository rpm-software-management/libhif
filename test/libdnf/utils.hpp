// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef TEST_LIBDNF_UTILS_HPP
#define TEST_LIBDNF_UTILS_HPP

#include "libdnf/advisory/advisory_query.hpp"
#include "libdnf/rpm/package_set.hpp"

#include <cppunit/extensions/HelperMacros.h>

#include <iterator>
#include <vector>


/// Implements assertion on std collections (e.g. asserting two vectors are equal).
/// https://stackoverflow.com/questions/43709391/cppunit-assert-equal-on-std-collections
namespace CPPUNIT_NS {
    template <template <typename...> class C, typename... T>
    struct assertion_traits<C<T...>> {
        inline static bool equal(const C<T...> & left, const C<T...> & right) {
            if (left.size() != right.size()) {
                return false;
            }
            return std::equal(
                left.cbegin(), left.cend(), right.cbegin(), right.cend(),
                assertion_traits<decltype(*(left.cbegin()))>::equal
            );
        }

        inline static std::string toString(const C<T...> & x) {
            std::ostringstream os;
            os << "[";
            std::transform(
                x.cbegin(), x.cend(), std::ostream_iterator<std::string>(os, ", "),
                assertion_traits<decltype(*(x.cbegin()))>::toString);
            os << "]";
            return os.str();
        }

        assertion_traits<C<T...>>() = delete;
        ~assertion_traits<C<T...>>() = delete;
        assertion_traits<C<T...>>(const assertion_traits<C<T...>>&) = delete;
        assertion_traits<C<T...>>& operator=(const assertion_traits<C<T...>>&) = delete;
    };
}


// CPPUNIT_ASSERT_EQUAL support for libdnf::rpm::Package
namespace CPPUNIT_NS {
    template <>
    struct assertion_traits<libdnf::rpm::Package> {
        inline static bool equal(const libdnf::rpm::Package & left, const libdnf::rpm::Package & right) {
            return left == right;
        }

        inline static std::string toString(const libdnf::rpm::Package & pkg) {
            return std::to_string(pkg.get_id().id);
        }
    };
}


/// Convert ReldepList to a vector of strings for easy assertions.
std::vector<std::string> to_vector_string(const libdnf::rpm::ReldepList & rdl);

/// Convert PackageSet to a vector of strings for easy assertions.
std::vector<std::string> to_vector_string(const libdnf::rpm::PackageSet & pset);

/// Convert vector<Package> to a vector of strings for easy assertions.
std::vector<std::string> to_vector_string(const std::vector<libdnf::rpm::Package> & pkg_list);

/// Convert AdvisoryQuery to a vector of strings of their names for easy assertions.
std::vector<std::string> to_vector_name_string(const libdnf::advisory::AdvisoryQuery & advisory_query);

#endif  // TEST_LIBDNF_UTILS_HPP
