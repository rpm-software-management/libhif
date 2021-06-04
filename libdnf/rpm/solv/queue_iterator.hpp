// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_RPM_SOLV_QUEUE_ITERATOR_HPP
#define LIBDNF_RPM_SOLV_QUEUE_ITERATOR_HPP

#include <solv/queue.h>

#include <cstddef>
#include <iterator>

namespace libdnf::rpm::solv {

class IdQueueIterator {
public:
    explicit IdQueueIterator(const Queue * queue) noexcept;
    IdQueueIterator(const IdQueueIterator & other) noexcept = default;

    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = Id;
    using pointer = void;
    using reference = Id;

    Id operator*() const noexcept { return queue->elements[current_index]; }

    IdQueueIterator & operator++() noexcept;
    IdQueueIterator operator++(int) noexcept;

    bool operator==(const IdQueueIterator & other) const noexcept { return current_index == other.current_index; }
    bool operator!=(const IdQueueIterator & other) const noexcept { return current_index != other.current_index; }

    void begin() noexcept { current_index = 0; }
    void end() noexcept { current_index = queue->count; }

protected:
    const Queue * get_queue() const noexcept { return queue; }

private:
    const Queue * queue;
    int current_index;
};

inline IdQueueIterator::IdQueueIterator(const Queue * queue) noexcept : queue{queue} {
    begin();
}

inline IdQueueIterator & IdQueueIterator::operator++() noexcept {
    ++current_index;
    return *this;
}

inline IdQueueIterator IdQueueIterator::operator++(int) noexcept {
    IdQueueIterator ret(*this);
    ++*this;
    return ret;
}

}  // namespace libdnf::rpm::solv

#endif  // LIBDNF_RPM_SOLV_QUEUE_ITERATOR_HPP
