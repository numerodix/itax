#ifndef LIBCORE_DEFS_H
#define LIBCORE_DEFS_H

#include <cstdint>

namespace core {

using RuleId = uint32_t;

enum class CreditDebit {
    CREDIT,
    DEBIT,
};

} // namespace core

#endif // LIBCORE_DEFS_H
