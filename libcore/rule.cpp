#include "rule.h"

namespace core {

Rule::Rule(RuleId rule_id, const std::string &slug, const std::string &desc,
           FnCalc fn)
    : m_rule_id{rule_id}, m_slug{slug}, m_desc{desc}, m_fn{fn} {}

Rule::~Rule() = default;

Rule::Rule(const Rule &other) = default;

Rule &Rule::operator=(const Rule &other) = default;

Rule::Rule(Rule &&other) = default;

Rule &Rule::operator=(Rule &&other) = default;

RuleId Rule::rule_id() const { return m_rule_id; }

const std::string &Rule::slug() const { return m_slug; }

const std::string &Rule::desc() const { return m_desc; }

RuleItems Rule::calculate(const TaxReturn &taxret,
                          const IncomeSlice &slice) const {
    auto items = m_fn(taxret, slice);
    return RuleItems{m_rule_id, items};
}

} // namespace core