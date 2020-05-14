#ifndef LIBCORE_INCOME_SLICE_H
#define LIBCORE_INCOME_SLICE_H

#include "cash_amount.h"

namespace core {

// Represents a slice of the total income.
//
// If the total income was 50k but it was earned from a few different income
// sources that paid 30k, 15k and 5k respectively, then treating each slice
// separately allows us to calculate the marginal tax on each slice separately
// as well.
//
// For the scenario above the three slices would be:
// - base: 0, income: 30k
// - base: 30k, income: 15k
// - base: 45k, income: 5k

class IncomeSlice {
  public:
    explicit IncomeSlice(CashAmount base, CashAmount amount);
    IncomeSlice();

    IncomeSlice(const IncomeSlice &other);
    IncomeSlice &operator=(const IncomeSlice &other);

    IncomeSlice(IncomeSlice &&other);
    IncomeSlice &operator=(IncomeSlice &&other);

    const CashAmount &base() const;
    const CashAmount &amount() const;

    CashAmount lower_bound() const;
    CashAmount upper_bound() const;

    friend IncomeSlice operator+(const IncomeSlice &left,
                                 const IncomeSlice &right);
    friend bool operator==(const IncomeSlice &left, const IncomeSlice &right);
    friend bool operator!=(const IncomeSlice &left, const IncomeSlice &right);

  private:
    CashAmount m_base;
    CashAmount m_amount;
};

// Convenience functions

template <typename Iterator> IncomeSlice sum(Iterator begin, Iterator end) {
    if (begin == end) {
        return IncomeSlice{};
    }

    IncomeSlice sum = *begin;
    for (auto it = ++begin; it != end; ++it) {
        sum = sum + *it;
    }

    return sum;
}

} // namespace core

#endif // LIBCORE_INCOME_SLICE_H
