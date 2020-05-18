#include "ruleset.h"

namespace rules {

Ruleset::Ruleset(const std::string &slug, const std::string &desc,
                 std::vector<core::Rule> rules)
    : m_slug{slug}, m_desc{desc}, m_rules{} {
    for (core::Rule &rule : rules) {
        m_rules[rule.rule_id()] = std::move(rule);
    }
}

Ruleset::Ruleset() = default;

Ruleset::~Ruleset() = default;

const std::string &Ruleset::slug() const { return m_slug; }

const std::string &Ruleset::desc() const { return m_desc; }

std::size_t Ruleset::num_rules() const { return m_rules.size(); }

const core::Rule &Ruleset::get_rule(core::RuleId rule_id) const {
    return m_rules.at(rule_id);
}

TaxCalculation Ruleset::apply(const core::TaxReturn &taxret) const {
    TaxCalculation calc{m_rules};

    for (const core::IncomeSlice &slice : taxret.slices()) {
        std::vector<core::RuleItems> vec_rule_items{};

        for (const auto &pair : m_rules) {
            const core::Rule &rule = pair.second;
            core::RuleItems rule_items = rule.calculate(taxret, slice);
            vec_rule_items.push_back(rule_items);
        }

        calc.add_slice(slice, vec_rule_items);
    }

    return calc;
}

} // namespace rules