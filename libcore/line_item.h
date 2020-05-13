#ifndef LIBCORE_LINE_ITEM_H
#define LIBCORE_LINE_ITEM_H

#include "cash_amount.h"

namespace core {

enum class CreditDebit {
    CREDIT,
    DEBIT,
};

class LineItem {
  public:
    LineItem(CreditDebit credit_debit, CashAmount amount);

    CreditDebit credit_debit() const;
    CashAmount amount() const;

  private:
    CreditDebit m_credit_debit;
    CashAmount m_amount;
};

} // namespace core

#endif // LIBCORE_LINE_ITEM_H
