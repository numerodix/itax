#include "rulesets_registry.h"

#include <algorithm>

#include "collections/aus.h"
#include "rules_registry.h"

namespace rules {

RulesetsRegistry *RulesetsRegistry::instance() {
    static RulesetsRegistry *registry{nullptr};

    if (registry == nullptr) {
        registry = new RulesetsRegistry();
        registry->populate_rulesets();
    }

    return registry;
}

RulesetsRegistry::RulesetsRegistry() = default;

RulesetsRegistry::~RulesetsRegistry() = default;

std::size_t RulesetsRegistry::num_rulesets() const { return m_rulesets.size(); }

std::vector<std::string> RulesetsRegistry::get_all_ruleset_ids() const {
    std::vector<std::string> ruleset_ids{};

    for (const auto& item: m_rulesets) {
        ruleset_ids.push_back(item.first);
    }

    std::sort(ruleset_ids.begin(), ruleset_ids.end());

    return ruleset_ids;
}

const Ruleset &RulesetsRegistry::get_ruleset(const std::string &slug) const {
    return m_rulesets.at(slug);
}

void RulesetsRegistry::populate_rulesets() {
    auto rules_registry = RulesRegistry::instance();

    for (auto tup : collections::RULESET_TUPLES) {
        auto slug = std::get<0>(tup);
        auto desc = std::get<1>(tup);
        auto rule_ids = std::get<2>(tup);

        std::vector<core::Rule> rules{};
        for (core::RuleId rule_id : rule_ids) {
            core::Rule rule = rules_registry->get_rule(rule_id);
            rules.push_back(rule);
        }

        Ruleset set{slug, desc, rules};
        m_rulesets[set.slug()] = set;
    }
}

} // namespace rules