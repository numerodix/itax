#ifndef LIBCORE_LINE_ITEM_H
#define LIBCORE_LINE_ITEM_H

#include "cash_amount.h"
#include "defs.h"

namespace core {

// Represents a line item in a tax calculation.

class LineItem {
  public:
    LineItem(CashAmount taxable, CashAmount payable, CreditDebit credit_debit);
    LineItem();
    ~LineItem();

    LineItem(const LineItem &other);
    LineItem &operator=(const LineItem &other);

    LineItem(LineItem &&other);
    LineItem &operator=(LineItem &&other);

    CashAmount taxable() const;
    CashAmount payable() const;
    CreditDebit credit_debit() const;

    double percent() const;
    CashAmount after_tax() const;

    friend LineItem operator+(const LineItem &left, const LineItem &right);
    friend bool operator==(const LineItem &left, const LineItem &right);
    friend bool operator!=(const LineItem &left, const LineItem &right);

  private:
    CashAmount m_taxable;
    CashAmount m_payable;
    CreditDebit m_credit_debit;
};

} // namespace core

#endif // LIBCORE_LINE_ITEM_H
