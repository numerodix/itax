#include "line_item.h"

#include <cstdint>
#include <cstdlib>
#include <stdexcept>

namespace core {

LineItem::LineItem(CashAmount taxable, CashAmount payable,
                   CreditDebit credit_debit)
    : m_taxable{taxable}, m_payable{payable}, m_credit_debit{credit_debit} {

    if ((m_payable > m_taxable) && (m_credit_debit == CreditDebit::DEBIT)) {
        throw std::out_of_range(
            "constructor called with a payable greater than the taxable");
    }
}

LineItem::LineItem()
    : m_taxable{CashAmount{0L}}, m_payable{CashAmount{0L}},
      m_credit_debit{CreditDebit::CREDIT} {}

LineItem::~LineItem() = default;

LineItem::LineItem(const LineItem &other) = default;

LineItem &LineItem::operator=(const LineItem &other) = default;

LineItem::LineItem(LineItem &&other) = default;

LineItem &LineItem::operator=(LineItem &&other) = default;

void LineItem::set_taxable(CashAmount taxable) { m_taxable = taxable; }

void LineItem::set_payable(CashAmount payable) { m_payable = payable; }

CashAmount LineItem::taxable() const { return m_taxable; }

CashAmount LineItem::payable() const { return m_payable; }

CreditDebit LineItem::credit_debit() const { return m_credit_debit; }

double LineItem::percent() const {
    double numerator = static_cast<double>(m_payable.raw());
    double denominator = static_cast<double>(m_taxable.raw());

    if (denominator == 0.0) {
        return 0.0;
    }

    return numerator / denominator;
}

CashAmount LineItem::after_tax() const {
    int64_t sign = m_credit_debit == CreditDebit::CREDIT ? 1L : -1L;
    int64_t result = m_taxable.raw() + (sign * m_payable.raw());
    return CashAmount{result};
}

LineItem operator+(const LineItem &left, const LineItem &right) {
    CashAmount taxable = left.m_taxable + right.m_taxable;

    int64_t left_pay = left.m_credit_debit == CreditDebit::CREDIT
                           ? left.m_payable.raw()
                           : -left.m_payable.raw();
    int64_t right_pay = right.m_credit_debit == CreditDebit::CREDIT
                            ? right.m_payable.raw()
                            : -right.m_payable.raw();

    int64_t payable = left_pay + right_pay;
    CreditDebit cd = payable >= 0 ? CreditDebit::CREDIT : CreditDebit::DEBIT;

    return LineItem{taxable, CashAmount{std::abs(payable)}, cd};
}

bool operator==(const LineItem &left, const LineItem &right) {
    return (left.m_credit_debit == right.m_credit_debit) &&
           (left.m_payable == right.m_payable);
}

bool operator!=(const LineItem &left, const LineItem &right) {
    return !(left == right);
}

} // namespace core