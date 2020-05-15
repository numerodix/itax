#include "tax_return.h"

#include <stdexcept>

#include "algos.h"

namespace core {

TaxReturn::TaxReturn(std::vector<IncomeSlice> slices) : m_slices{slices} {
    validate_slices();
}

TaxReturn::~TaxReturn() = default;

TaxReturn::TaxReturn(const TaxReturn &other) = default;

TaxReturn &TaxReturn::operator=(const TaxReturn &other) = default;

TaxReturn::TaxReturn(TaxReturn &&other) = default;

TaxReturn &TaxReturn::operator=(TaxReturn &&other) = default;

const std::vector<IncomeSlice> &TaxReturn::slices() const { return m_slices; }

CashAmount TaxReturn::total_income() const {
    IncomeSlice summed = sum(m_slices.begin(), m_slices.end(), IncomeSlice{});
    return summed.amount();
}

void TaxReturn::validate_slices() const {
    IncomeSlice summed = sum(m_slices.begin(), m_slices.end(), IncomeSlice{});

    if (summed.base() != CashAmount(0L)) {
        throw std::invalid_argument("sum of slices must have a base of 0");
    }
}

} // namespace core