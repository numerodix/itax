#ifndef LIBCORE_TAX_RETURN_H
#define LIBCORE_TAX_RETURN_H

#include <vector>

#include "income_slice.h"

namespace core {

// Represents the information captured in a tax return. This is used as input
// into the tax calculation.
//
// FIXME: flesh out

class TaxReturn {
  public:
    TaxReturn(std::vector<IncomeSlice> slices);

    TaxReturn(const TaxReturn &other) = delete;
    TaxReturn &operator=(const TaxReturn &other) = delete;

    TaxReturn(TaxReturn &&other) = default;
    TaxReturn &operator=(TaxReturn &&other) = delete;

    const std::vector<IncomeSlice> &slices() const { return m_slices; }

    CashAmount total_income() const;

  private:
    void validate_slices() const;

    std::vector<IncomeSlice> m_slices;
};

} // namespace core

#endif // LIBCORE_TAX_RETURN_H