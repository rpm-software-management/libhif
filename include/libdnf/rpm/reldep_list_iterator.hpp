// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_RPM_RELDEP_LIST_ITERATOR_HPP
#define LIBDNF_RPM_RELDEP_LIST_ITERATOR_HPP


#include "reldep.hpp"

#include <cstddef>
#include <iterator>
#include <memory>


namespace libdnf::rpm {


class ReldepList;


class ReldepListIterator {
public:
    explicit ReldepListIterator(const ReldepList & reldep_list);
    ReldepListIterator(const ReldepListIterator & other);
    ~ReldepListIterator();

    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = Reldep;
    using pointer = void;
    using reference = Reldep;

    Reldep operator*();

    ReldepListIterator & operator++();
    ReldepListIterator operator++(int);

    bool operator==(const ReldepListIterator & other) const;
    bool operator!=(const ReldepListIterator & other) const;

    void begin();
    void end();

private:
    class Impl;
    std::unique_ptr<Impl> p_impl;
};


}  // namespace libdnf::rpm


#endif  // LIBDNF_RPM_RELDEP_LIST_ITERATOR_HPP
