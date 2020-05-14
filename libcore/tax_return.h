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

    CashAmount total_income() const;

  private:
    void validate_slices() const;
    IncomeSlice sum_slices() const;

    std::vector<IncomeSlice> m_slices;
};

} // namespace core

#endif // LIBCORE_TAX_RETURN_H