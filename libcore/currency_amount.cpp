#include <cerrno>
#include <cstring>
#include <sstream>
#include <stdexcept>

#include "currency_amount.h"

namespace core {

CurrencyAmount CurrencyAmount::from(std::string amount) {
    double value_part_f = std::strtod(amount.c_str(), nullptr);

    // NOTE: does not handle valid inputs with leading zeroes :/
    if ((value_part_f == 0.0) && ((amount != "0") && (amount != "0.0"))) {
        throw std::invalid_argument("from() called with an input that could "
                                    "not be parsed as an amount");
    }

    int64_t value_part = static_cast<int64_t>(value_part_f * 100.0) * 100L;
    return CurrencyAmount{value_part};
}

CurrencyAmount::CurrencyAmount(int64_t amount) : m_amount{amount} {
    if (amount < 0L) {
        throw std::out_of_range("constructor called with a negative number");
    }
}

int64_t CurrencyAmount::raw() const { return m_amount; }

int64_t CurrencyAmount::value_part() const { return m_amount / 100L; }

int64_t CurrencyAmount::rounding_part() const {
    return m_amount - (value_part() * 100L);
}

int64_t CurrencyAmount::rounded_value_part() const {
    int64_t val_part = value_part();
    return rounding_part() >= 50 ? val_part + 1L : val_part;
}

std::string CurrencyAmount::display_plain() const {
    int64_t value_part = rounded_value_part();

    int64_t int_part = value_part / 100;
    int64_t dec_part = value_part - (int_part * 100);

    std::stringstream ss{};
    ss << int_part << '.' << dec_part;
    return ss.str();
}

std::string CurrencyAmount::display_with_commas() const {
    std::string plain = display_plain();

    std::stringstream ss{};
    int counter = 0;
    bool past_decimal_point = false;

    for (auto it = plain.rbegin(); it != plain.rend(); ++it) {
        if (past_decimal_point) {
            if ((counter > 0) && (counter % 3 == 0)) {
                ss << ',';
            }

            ss << *it;
            ++counter;

        } else {
            if (*it == '.') {
                past_decimal_point = true;
            }

            ss << *it;
        }
    }

    std::string backwards = ss.str();
    std::string forwards(backwards.rbegin(), backwards.rend());
    return forwards;
}

bool operator==(const CurrencyAmount &left, const CurrencyAmount &right) {
    return left.m_amount == right.m_amount;
}

bool operator!=(const CurrencyAmount &left, const CurrencyAmount &right) {
    return left.m_amount != right.m_amount;
}

bool operator<(const CurrencyAmount &left, const CurrencyAmount &right) {
    return left.m_amount < right.m_amount;
}

bool operator>(const CurrencyAmount &left, const CurrencyAmount &right) {
    return left.m_amount > right.m_amount;
}

bool operator<=(const CurrencyAmount &left, const CurrencyAmount &right) {
    return left.m_amount <= right.m_amount;
}

bool operator>=(const CurrencyAmount &left, const CurrencyAmount &right) {
    return left.m_amount >= right.m_amount;
}

CurrencyAmount operator+(const CurrencyAmount &left,
                         const CurrencyAmount &right) {
    int64_t res = left.m_amount + right.m_amount;
    return CurrencyAmount{res};
}

CurrencyAmount operator-(const CurrencyAmount &left,
                         const CurrencyAmount &right) {
    int64_t res = left.m_amount - right.m_amount;
    return CurrencyAmount{res};
}

CurrencyAmount operator*(const CurrencyAmount &left, double right) {
    double left_f = static_cast<double>(left.m_amount);
    double res_f = left_f * right;
    int64_t res = static_cast<int64_t>(res_f);
    return CurrencyAmount{res};
}

CurrencyAmount operator*(double left, const CurrencyAmount& right) {
    double right_f = static_cast<double>(right.m_amount);
    double res_f = left * right_f;
    int64_t res = static_cast<int64_t>(res_f);
    return CurrencyAmount{res};
}

CurrencyAmount operator/(const CurrencyAmount &left, double right) {
    double left_f = static_cast<double>(left.m_amount);
    double res_f = left_f / right;
    int64_t res = static_cast<int64_t>(res_f);
    return CurrencyAmount{res};
}

} // namespace core