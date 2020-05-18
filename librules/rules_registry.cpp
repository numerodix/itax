#include "rules_registry.h"

#include "db/aus.h"

namespace rules {

RulesRegistry* RulesRegistry::instance() {
    static RulesRegistry* registry = new RulesRegistry();

    if (registry == nullptr) {
        registry = new RulesRegistry();
        registry->populate_rules();
    }

    return registry;
}

RulesRegistry::RulesRegistry() = default;

void RulesRegistry::populate_rules() {
    auto id = db::AUS_BRACKET_1;
    m_rules[id] = db::get_bracket_1(id);
}

} // namespace rules