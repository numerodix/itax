#ifndef LIBCORE_RULE_H
#define LIBCORE_RULE_H

#include <functional>
#include <stdexcept>

#include "defs.h"
#include "income_slice.h"
#include "line_item.h"
#include "rule_items.h"
#include "tax_return.h"

namespace core {

class Bracket {
  public:
    Bracket(CashAmount lower, CashAmount upper)
        : m_lower{lower}, m_upper{upper} {}

    CashAmount in_bracket(const IncomeSlice &slice) const {
        // the slice is entirely below or above the bracket
        if ((slice.upper_bound() < m_lower) ||
            (m_upper < slice.lower_bound())) {
            return CashAmount{0L};
        }

        // the slice is entirely contained between the bounds of the bracket
        if ((m_lower < slice.lower_bound()) &&
            (slice.upper_bound() < m_upper)) {
            return slice.amount();
        }

        // the slice overlaps or exceeds the whole bracket
        if ((slice.lower_bound() <= m_lower) &&
            (m_upper <= slice.upper_bound())) {
            return m_upper - m_lower;
        }

        // the slice begins before the bracket and ends in the bracket
        if ((slice.lower_bound() < m_lower) &&
            (slice.upper_bound() < m_upper)) {
            return slice.upper_bound() - m_lower;
        }

        // the slice begins inside the bracket and ends after the bracket
        if ((m_lower < slice.lower_bound()) &&
            (m_upper < slice.upper_bound())) {
            return m_upper - slice.lower_bound();
        }

        throw std::runtime_error("should not reach this point");
    }

  private:
    CashAmount m_lower;
    CashAmount m_upper;
};

class Context {
  public:
    Context(const TaxReturn &taxret) : m_taxret{taxret} {}

    const TaxReturn &tax_return() const { return m_taxret; }

  private:
    const TaxReturn &m_taxret;
};

using FnCalcForSlice =
    std::function<std::vector<LineItem>(const Context &, const IncomeSlice &)>;

#define FN_CALC_SLICE_SIG                                                      \
    [=](const Context &context,                                                \
        const IncomeSlice &slice) -> std::vector<LineItem>

class Rule {
  public:
    Rule(RuleId rule_id, const std::string &slug, const std::string &desc,
         FnCalcForSlice fn)
        : m_rule_id{rule_id}, m_slug{slug}, m_desc{desc}, m_fn{fn} {}

    RuleId rule_id() const { return m_rule_id; }

    const std::string &slug() const { return m_slug; }

    const std::string &desc() const { return m_desc; }

    RuleItems calculate(const Context &context,
                        const IncomeSlice &slice) const {
        auto items = m_fn(context, slice);
        return RuleItems{m_rule_id, items};
    }

  private:
    RuleId m_rule_id;
    std::string m_slug;
    std::string m_desc;
    FnCalcForSlice m_fn;
};

} // namespace core

#endif // LIBCORE_RULE_H
