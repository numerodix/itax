#ifndef LIBCORE_CASH_AMOUNT
#define LIBCORE_CASH_AMOUNT

#include <cstdint>
#include <string>

namespace core {

// Represents a cash amount.
//
// Stored as an int64_t. All amounts have to be positive - the sign is used to
// detect overflow during arithmetic operations.
//
// The amount 23.95 is represented as:
//   239500
// The first four digits are the value_part. They represent the actual amount.
// The last two digits are the rounding_part. They are used to preserve more
// precision when rounding.

class CashAmount {
  public:
    static CashAmount from(std::string amount);

    explicit CashAmount(int64_t amount);
    ~CashAmount();

    CashAmount(const CashAmount &other);
    CashAmount &operator=(const CashAmount &other);

    CashAmount(CashAmount &&other);
    CashAmount &operator=(CashAmount &&other);

    int64_t raw() const;
    int64_t value_part() const;
    int64_t rounding_part() const;
    int64_t rounded_value_part() const;

    std::string display_plain() const;
    std::string display_with_commas() const;

    friend bool operator==(const CashAmount &left, const CashAmount &right);
    friend bool operator!=(const CashAmount &left, const CashAmount &right);
    friend bool operator<(const CashAmount &left, const CashAmount &right);
    friend bool operator>(const CashAmount &left, const CashAmount &right);
    friend bool operator<=(const CashAmount &left, const CashAmount &right);
    friend bool operator>=(const CashAmount &left, const CashAmount &right);

    friend CashAmount operator+(const CashAmount &left,
                                const CashAmount &right);
    friend CashAmount operator-(const CashAmount &left,
                                const CashAmount &right);
    friend CashAmount operator*(const CashAmount &left, double right);
    friend CashAmount operator*(double left, const CashAmount &right);
    friend CashAmount operator/(const CashAmount &left, double right);

  private:
    int64_t m_amount{};
};

} // namespace core

#endif // LIBCORE_CASH_AMOUNT