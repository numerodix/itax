#ifndef LIBTAX_CURRENCY_AMOUNT
#define LIBTAX_CURRENCY_AMOUNT

#include <cstdint>
#include <string>

namespace core {

// Represents a cash amount in a currency.
//
// Stored as an int64_t. All amounts have to be positive - the sign is used to
// detect overflow during arithmetic operations.
//
// The amount 23.95 is represented as:
//   239500
// The first four digits are the value_part. They represent the actual amount.
// The last two digits are the rounding_part. They are used to preserve more
// precision when rounding.

class CurrencyAmount {
  public:
    static CurrencyAmount from(std::string amount);

    explicit CurrencyAmount(int64_t amount);

    int64_t raw() const;
    int64_t value_part() const;
    int64_t rounding_part() const;

    int64_t rounded_value_part() const;

    std::string display_plain() const;
    std::string display_with_commas() const;

  private:
    int64_t m_amount{};
};

} // namespace core

#endif // LIBTAX_CURRENCY_AMOUNT