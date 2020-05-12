#ifndef LIBTAX_CURRENCY_AMOUNT
#define LIBTAX_CURRENCY_AMOUNT

#include <cstdint>

namespace core {

class CurrencyAmount {
  public:
    explicit CurrencyAmount(int64_t amount);

    int64_t raw() const;

  private:
    int64_t m_amount{};
};

} // namespace core

#endif // LIBTAX_CURRENCY_AMOUNT