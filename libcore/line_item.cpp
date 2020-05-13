#include "line_item.h"

namespace core {

LineItem::LineItem(CreditDebit credit_debit, CashAmount amount)
    : m_credit_debit{credit_debit}, m_amount{amount} {}

CreditDebit LineItem::credit_debit() const { return m_credit_debit; }

CashAmount LineItem::amount() const { return m_amount; }

} // namespace core