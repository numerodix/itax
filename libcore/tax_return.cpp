#include "tax_return.h"

#include <stdexcept>

namespace core {

TaxReturn::TaxReturn(std::vector<IncomeSlice> slices) : m_slices{slices} {
    validate_slices();
}

CashAmount TaxReturn::total_income() const {
    IncomeSlice sum = sum_slices();
    return sum.upper_bound();
}

void TaxReturn::validate_slices() const {
    IncomeSlice sum = sum_slices(); // will throw if slices not adjacent

    if (sum.base() > CashAmount(0L)) {
        throw std::invalid_argument("sum of slices must have a base of 0");
    }
}

IncomeSlice TaxReturn::sum_slices() const {
    if (m_slices.size() == 0) {
        return IncomeSlice{};
    }

    IncomeSlice sum = m_slices[0];

    for (auto it = m_slices.begin() + 1; it != m_slices.end(); ++it) {
        sum = sum + *it;
    }

    return sum;
}

} // namespace core