#include "rule.h"

namespace core {

RuleItems::RuleItems(RuleId rule_id, std::vector<LineItem> items)
    : m_rule_id{rule_id}, m_items{items} {}

RuleId RuleItems::rule_id() const { return m_rule_id; }

std::vector<LineItem> RuleItems::items() const { return m_items; }

Rule::Rule(RuleId rule_id, const std::string &slug, const std::string &desc,
           FnCalcForSlice fn)
    : m_rule_id{rule_id}, m_slug{slug}, m_desc{desc}, m_fn{fn} {}

RuleItems Rule::calculate_for_slice(const TaxReturn &taxret,
                                    const IncomeSlice &slice) {
    auto items = m_fn(taxret, slice);
    return RuleItems{m_rule_id, items};
}

} // namespace core