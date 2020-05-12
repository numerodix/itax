#include "currency_amount.h"

namespace core {

CurrencyAmount::CurrencyAmount(int64_t amount) : m_amount{amount} {}

int64_t CurrencyAmount::raw() const {
    return m_amount;
}

} // namespace core