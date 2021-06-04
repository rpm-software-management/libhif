// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_RPM_PACKAGE_SET_ITERATOR_HPP
#define LIBDNF_RPM_PACKAGE_SET_ITERATOR_HPP


#include "package.hpp"

#include <cstddef>
#include <iterator>
#include <memory>


namespace libdnf::rpm {


class PackageSet;


class PackageSetIterator {
public:
    explicit PackageSetIterator(const PackageSet & package_set);
    PackageSetIterator(const PackageSetIterator & other);
    ~PackageSetIterator();

    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = Package;
    using pointer = void;
    using reference = Package;

    Package operator*();

    PackageSetIterator & operator++();
    PackageSetIterator operator++(int);

    bool operator==(const PackageSetIterator & other) const;
    bool operator!=(const PackageSetIterator & other) const;

    void begin();
    void end();

private:
    class Impl;
    std::unique_ptr<Impl> p_impl;
};


}  // namespace libdnf::rpm


#endif  // LIBDNF_RPM_PACKAGE_SET_ITERATOR_HPP
