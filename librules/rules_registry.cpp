#include "rules_registry.h"

#include "collections/aus.h"

namespace rules {

RulesRegistry *RulesRegistry::instance() {
    static RulesRegistry *registry = new RulesRegistry();

    if (registry == nullptr) {
        registry = new RulesRegistry();
        registry->populate_rules();
    }

    return registry;
}

RulesRegistry::RulesRegistry() = default;

RulesRegistry::~RulesRegistry() = default;

void RulesRegistry::populate_rules() {
    for (auto fn : collections::RULE_FACTORIES) {
        core::Rule rule = fn();
        m_rules[rule.rule_id()] = rule;
    }
}

} // namespace rules