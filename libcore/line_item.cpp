#include "line_item.h"

#include <cstdint>
#include <cstdlib>

namespace core {

LineItem::LineItem(CreditDebit credit_debit, CashAmount amount)
    : m_credit_debit{credit_debit}, m_amount{amount} {}

LineItem::LineItem()
    : m_credit_debit{CreditDebit::CREDIT}, m_amount{CashAmount{0L}} {}

LineItem::~LineItem() = default;

LineItem::LineItem(const LineItem &other) = default;

LineItem &LineItem::operator=(const LineItem &other) = default;

LineItem::LineItem(LineItem &&other) = default;

LineItem &LineItem::operator=(LineItem &&other) = default;

CreditDebit LineItem::credit_debit() const { return m_credit_debit; }

CashAmount LineItem::amount() const { return m_amount; }

LineItem operator+(const LineItem &left, const LineItem &right) {
    auto left_amt = left.m_credit_debit == CreditDebit::CREDIT
                        ? left.m_amount.raw()
                        : -left.m_amount.raw();
    auto right_amt = right.m_credit_debit == CreditDebit::CREDIT
                         ? right.m_amount.raw()
                         : -right.m_amount.raw();

    auto sum = left_amt + right_amt;
    CreditDebit cd = sum >= 0 ? CreditDebit::CREDIT : CreditDebit::DEBIT;

    return LineItem{cd, CashAmount{std::abs(sum)}};
}

bool operator==(const LineItem &left, const LineItem &right) {
    return (left.m_credit_debit == right.m_credit_debit) &&
           (left.m_amount == right.m_amount);
}

bool operator!=(const LineItem &left, const LineItem &right) {
    return !(left == right);
}

} // namespace core