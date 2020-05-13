#ifndef LIBCORE_INCOME_SLICE_H
#define LIBCORE_INCOME_SLICE_H

#include "cash_amount.h"

namespace core {

class IncomeSlice {
  public:
    explicit IncomeSlice(CashAmount base, CashAmount amount);
    IncomeSlice();

    const CashAmount &base() const;
    const CashAmount &amount() const;

    CashAmount lower_bound() const;
    CashAmount upper_bound() const;

    friend IncomeSlice operator+(const IncomeSlice &left,
                                 const IncomeSlice &right);

  private:
    CashAmount m_base;
    CashAmount m_amount;
};

} // namespace core

#endif // LIBCORE_INCOME_SLICE_H
