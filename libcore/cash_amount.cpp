#include "cash_amount.h"

#include <cerrno>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace core {

CashAmount CashAmount::from(std::string amount) {
    const char *buf = amount.c_str();
    char *cursor = const_cast<char *>(buf);

    errno = 0;
    double value_part_f = std::strtod(buf, &cursor);
    auto num_read = static_cast<std::size_t>(cursor - buf);

    if ((errno > 0) || (num_read != amount.size())) {
        throw std::invalid_argument("from() called with an input that could "
                                    "not be parsed as an amount");
    }

    int64_t value_part = static_cast<int64_t>(value_part_f * 100.0) * 100L;
    return CashAmount{value_part};
}

CashAmount::CashAmount(int64_t amount) {
    if (amount < 0L) {
        throw std::out_of_range("constructor called with a negative number");
    }

    m_amount = amount;
}

CashAmount::~CashAmount() = default;

CashAmount::CashAmount(const CashAmount &other) = default;

CashAmount &CashAmount::operator=(const CashAmount &other) = default;

CashAmount::CashAmount(CashAmount &&other) = default;

CashAmount &CashAmount::operator=(CashAmount &&other) = default;

int64_t CashAmount::raw() const { return m_amount; }

int64_t CashAmount::value_part() const { return m_amount / 100L; }

int64_t CashAmount::rounding_part() const {
    return m_amount - (value_part() * 100L);
}

int64_t CashAmount::rounded_value_part() const {
    int64_t val_part = value_part();
    return rounding_part() >= 50 ? val_part + 1L : val_part;
}

std::string CashAmount::display_plain() const {
    int64_t value_part = rounded_value_part();

    int64_t int_part = value_part / 100;
    int64_t dec_part = value_part - (int_part * 100);

    std::stringstream ss{};
    ss << int_part << '.' << std::setfill('0') << std::setw(2) << dec_part;
    return ss.str();
}

std::string CashAmount::display_with_commas() const {
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

bool operator==(const CashAmount &left, const CashAmount &right) {
    return left.m_amount == right.m_amount;
}

bool operator!=(const CashAmount &left, const CashAmount &right) {
    return left.m_amount != right.m_amount;
}

bool operator<(const CashAmount &left, const CashAmount &right) {
    return left.m_amount < right.m_amount;
}

bool operator>(const CashAmount &left, const CashAmount &right) {
    return left.m_amount > right.m_amount;
}

bool operator<=(const CashAmount &left, const CashAmount &right) {
    return left.m_amount <= right.m_amount;
}

bool operator>=(const CashAmount &left, const CashAmount &right) {
    return left.m_amount >= right.m_amount;
}

CashAmount operator+(const CashAmount &left, const CashAmount &right) {
    int64_t res = left.m_amount + right.m_amount;
    return CashAmount{res};
}

CashAmount operator-(const CashAmount &left, const CashAmount &right) {
    int64_t res = left.m_amount - right.m_amount;
    return CashAmount{res};
}

CashAmount operator*(const CashAmount &left, double right) {
    double left_f = static_cast<double>(left.m_amount);
    double res_f = left_f * right;
    int64_t res = static_cast<int64_t>(res_f);
    return CashAmount{res};
}

CashAmount operator*(double left, const CashAmount &right) {
    double right_f = static_cast<double>(right.m_amount);
    double res_f = left * right_f;
    int64_t res = static_cast<int64_t>(res_f);
    return CashAmount{res};
}

CashAmount operator/(const CashAmount &left, double right) {
    double left_f = static_cast<double>(left.m_amount);
    double res_f = left_f / right;
    int64_t res = static_cast<int64_t>(res_f);
    return CashAmount{res};
}

} // namespace core