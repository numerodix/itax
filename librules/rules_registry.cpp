#include "rules_registry.h"

#include "collections/aus.h"

namespace rules {

RulesRegistry *RulesRegistry::instance() {
    static RulesRegistry *registry{nullptr};

    if (registry == nullptr) {
        registry = new RulesRegistry();
        registry->populate_rules();
    }

    return registry;
}

RulesRegistry::RulesRegistry() = default;

RulesRegistry::~RulesRegistry() = default;

std::size_t RulesRegistry::num_rules() const { return m_rules.size(); }

const core::Rule &RulesRegistry::get_rule(core::RuleId rule_id) const {
    return m_rules.at(rule_id);
}

void RulesRegistry::populate_rules() {
    for (auto fn : collections::RULE_FACTORIES) {
        core::Rule rule = fn();
        m_rules[rule.rule_id()] = rule;
    }
}

} // namespace rules