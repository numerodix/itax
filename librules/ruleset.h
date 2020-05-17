#ifndef LIBRULES_RULESET_H
#define LIBRULES_RULESET_H

#include <string>
#include <unordered_map>

#include "libcore/rule.h"

namespace rules {

// Represents a complete set of tax rules for a given tax year.

class RuleSet {
  public:
    RuleSet(const std::string &slug, const std::string &desc,
            std::vector<core::Rule> rules);

    const std::string &slug() const;
    const std::string &desc() const;
    std::size_t num_rules() const;

    const core::Rule &get_rule(core::RuleId rule_id) const;

  private:
    std::string m_slug;
    std::string m_desc;
    std::unordered_map<core::RuleId, core::Rule> m_rules;
};

} // namespace rules

#endif // LIBRULES_RULESET_H
