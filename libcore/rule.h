#ifndef LIBCORE_RULE_H
#define LIBCORE_RULE_H

#include <cstdint>
#include <vector>

#include "cash_amount.h"
#include "line_item.h"
#include "tax_return.h"

namespace core {

using RuleId = uint32_t;
using FnCalcForSlice = std::vector<LineItem> (*)(const TaxReturn &,
                                                 const IncomeSlice &);

class RuleItems {
  public:
    RuleItems(RuleId rule_id, std::vector<LineItem> items);

    RuleId rule_id() const;
    std::vector<LineItem> items() const;

  private:
    RuleId m_rule_id;
    std::vector<LineItem> m_items;
};

class Rule {
  public:
    Rule(RuleId rule_id, const std::string &slug, const std::string &desc,
         FnCalcForSlice fn);

    RuleItems calculate_for_slice(const TaxReturn &taxret,
                                  const IncomeSlice &slice);

  private:
    RuleId m_rule_id;
    std::string m_slug;
    std::string m_desc;
    FnCalcForSlice m_fn;
};

} // namespace core

#endif // LIBCORE_RULE_H
