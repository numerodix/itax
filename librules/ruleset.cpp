#include "ruleset.h"

namespace rules {

RuleSet::RuleSet(const std::string &slug, const std::string &desc,
                 std::vector<core::Rule> rules)
    : m_slug{slug}, m_desc{desc}, m_rules{} {
    for (core::Rule &rule : rules) {
        m_rules[rule.rule_id()] = std::move(rule);
    }
}

const std::string &RuleSet::slug() const { return m_slug; }

const std::string &RuleSet::desc() const { return m_desc; }

std::size_t RuleSet::num_rules() const { return m_rules.size(); }

const core::Rule &RuleSet::get_rule(core::RuleId rule_id) const {
    return m_rules.at(rule_id);
}

} // namespace rules