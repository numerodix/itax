#include "income_slice.h"

#include <stdexcept>

namespace core {

IncomeSlice::IncomeSlice(CashAmount base, CashAmount amount)
    : m_base{base}, m_amount{amount} {}

IncomeSlice::IncomeSlice() : m_base{CashAmount{0L}}, m_amount{CashAmount{0L}} {}

IncomeSlice::IncomeSlice(const IncomeSlice &other) = default;
IncomeSlice &IncomeSlice::operator=(const IncomeSlice &other) = default;

IncomeSlice::IncomeSlice(IncomeSlice &&other) = default;
IncomeSlice &IncomeSlice::operator=(IncomeSlice &&other) = default;

const CashAmount &IncomeSlice::base() const { return m_base; }

const CashAmount &IncomeSlice::amount() const { return m_amount; }

CashAmount IncomeSlice::lower_bound() const { return m_base; }

CashAmount IncomeSlice::upper_bound() const { return m_base + m_amount; }

IncomeSlice operator+(const IncomeSlice &left, const IncomeSlice &right) {
    IncomeSlice *lower = const_cast<IncomeSlice *>(&left);
    IncomeSlice *upper = const_cast<IncomeSlice *>(&right);

    if (left.m_base > right.m_base) {
        upper = const_cast<IncomeSlice *>(&left);
        lower = const_cast<IncomeSlice *>(&right);
    }

    if (lower->upper_bound() != upper->m_base) {
        throw std::invalid_argument("cannot add non-adjacent slices");
    }

    return IncomeSlice{lower->m_base, lower->m_amount + upper->m_amount};
}

bool operator==(const IncomeSlice &left, const IncomeSlice &right) {
    return (left.m_base == right.m_base) && (left.m_amount == right.m_amount);
}

bool operator!=(const IncomeSlice &left, const IncomeSlice &right) {
    return !(left == right);
}

} // namespace core