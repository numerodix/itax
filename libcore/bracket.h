#ifndef LIBCORE_BRACKET_H
#define LIBCORE_BRACKET_H

#include "cash_amount.h"
#include "income_slice.h"

namespace core {

class Bracket {
  public:
    Bracket(CashAmount lower, CashAmount upper);
    ~Bracket();

    Bracket(const Bracket &other);
    Bracket &operator=(const Bracket &other);

    Bracket(Bracket &&other);
    Bracket &operator=(Bracket &&other);

    CashAmount lower() const;
    CashAmount upper() const;

    // Calculates the amount of the `slice` that is contained inside this
    // bracket.
    CashAmount in_bracket(const IncomeSlice &slice) const;

  private:
    CashAmount m_lower;
    CashAmount m_upper;
};

} // namespace core

#endif // LIBCORE_BRACKET_H
