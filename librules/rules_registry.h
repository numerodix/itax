#ifndef LIBRULES_RULES_REGISTRY_H
#define LIBRULES_RULES_REGISTRY_H

#include <cstdint>
#include <unordered_map>

#include "libcore/defs.h"
#include "libcore/rule.h"

namespace rules {

class RulesRegistry {
  public:
    static RulesRegistry *instance();

    RulesRegistry();
    ~RulesRegistry();

    std::size_t num_rules() const;
    const core::Rule &get_rule(core::RuleId rule_id) const;

  private:
    void populate_rules();

    std::unordered_map<core::RuleId, core::Rule> m_rules{};
};

} // namespace rules

#endif // LIBRULES_RULES_REGISTRY_H
