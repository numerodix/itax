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

using FnCalc = std::function<std::vector<LineItem>(const TaxReturn &,
                                                   const IncomeSlice &)>;

#define FN_CALC_SIGNATURE                                                      \
    [=]([[maybe_unused]] const TaxReturn &taxret,                              \
        [[maybe_unused]] const IncomeSlice &slice) -> std::vector<LineItem>

// Represents a tax rule.

class Rule {
  public:
    Rule(RuleId rule_id, const std::string &slug, const std::string &desc,
         FnCalc fn);
    ~Rule();

    Rule(const Rule &other);
    Rule &operator=(const Rule &other);

    Rule(Rule &&other);
    Rule &operator=(Rule &&other);

    RuleId rule_id() const;
    const std::string &slug() const;
    const std::string &desc() const;

    // Applies the rule on the `slice`.
    RuleItems calculate(const TaxReturn &taxret,
                        const IncomeSlice &slice) const;

  private:
    RuleId m_rule_id;
    std::string m_slug;
    std::string m_desc;
    FnCalc m_fn;
};

} // namespace core

#endif // LIBCORE_RULE_H
