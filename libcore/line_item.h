#ifndef LIBCORE_LINE_ITEM_H
#define LIBCORE_LINE_ITEM_H

#include "cash_amount.h"
#include "defs.h"

namespace core {

// Represents a line item on an invoice.

class LineItem {
  public:
    LineItem(CreditDebit credit_debit, CashAmount amount);
    LineItem();
    ~LineItem();

    LineItem(const LineItem &other);
    LineItem &operator=(const LineItem &other);

    LineItem(LineItem &&other);
    LineItem &operator=(LineItem &&other);

    CreditDebit credit_debit() const;
    CashAmount amount() const;

    friend LineItem operator+(const LineItem &left, const LineItem &right);
    friend bool operator==(const LineItem &left, const LineItem &right);
    friend bool operator!=(const LineItem &left, const LineItem &right);

  private:
    CreditDebit m_credit_debit;
    CashAmount m_amount;
};

// Convenience functions

template <typename Iterator> LineItem sum(Iterator begin, Iterator end) {
    if (begin == end) {
        return LineItem{};
    }

    LineItem sum = *begin;
    for (auto it = ++begin; it != end; ++it) {
        sum = sum + *it;
    }

    return sum;
}

} // namespace core

#endif // LIBCORE_LINE_ITEM_H
