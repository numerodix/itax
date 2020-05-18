#ifndef LIBCORE_DEFS_H
#define LIBCORE_DEFS_H

#include <cstdint>

namespace core {

// An opaque unique identifier for each `Rule` instance.
using RuleId = uint64_t;

enum class CreditDebit {
    CREDIT,
    DEBIT,
};

} // namespace core

#endif // LIBCORE_DEFS_H
