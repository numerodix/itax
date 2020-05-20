#include "cash_amount.h"

#include <cerrno>
#include <cstring>
#include <iomanip>
#include <limits>
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

CashAmount CashAmount::max() {
    return CashAmount{std::numeric_limits<int64_t>::max()};
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

void CashAmount::zero_rounding_part() { m_amount = (m_amount / 100) * 100; }

int64_t CashAmount::raw() const { return m_amount; }

int64_t CashAmount::value_part() const { return m_amount / 100L; }

int64_t CashAmount::rounding_part() const {
    return m_amount - (value_part() * 100L);
}

int64_t CashAmount::rounded_value_part() const {
    int64_t val_part = value_part();
    return rounding_part() >= 50 ? val_part + 1L : val_part;
}

int64_t CashAmount::dollars() const {
    int64_t val_part = rounded_value_part();
    return val_part / 100L;
}

int64_t CashAmount::cents() const {
    int64_t val_part = rounded_value_part();
    int64_t whole_dollars = dollars();
    return val_part - (whole_dollars * 100L);
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

CashAmount operator*(const CashAmount &left, const CashAmount &right) {
    int64_t res = left.m_amount * right.m_amount;
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

double operator/(const CashAmount &left, const CashAmount &right) {
    double ratio =
        static_cast<double>(left.raw()) / static_cast<double>(right.raw());
    return ratio;
}

CashAmount operator/(const CashAmount &left, double right) {
    double left_f = static_cast<double>(left.m_amount);
    double res_f = left_f / right;
    int64_t res = static_cast<int64_t>(res_f);
    return CashAmount{res};
}

std::ostream &operator<<(std::ostream &out, const CashAmount &amt) {
    out << amt.raw();
    return out;
}

} // namespace core