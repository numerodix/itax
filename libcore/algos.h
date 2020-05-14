#ifndef LIBCORE_ALGOS_H
#define LIBCORE_ALGOS_H

#include "cash_amount.h"

namespace core {

// Like std::accumulate but avoids adding the `zero` value to the elements in
// the container if the container is non-empty. `zero` is explicitly a zero
// value, so we can do this without violating the semantics.

template <typename T, typename Iterator>
T sum(Iterator begin, Iterator end, T zero) {
    if (begin == end) {
        return zero;
    }

    T sum = *begin;
    for (auto it = ++begin; it != end; ++it) {
        sum = sum + *it;
    }

    return sum;
}

} // namespace core

#endif // LIBCORE_ALGOS_H
