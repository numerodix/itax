#ifndef LIBRULES_RULESETS_REGISTRY_H
#define LIBRULES_RULESETS_REGISTRY_H

#include <string>
#include <unordered_map>

#include "libcore/defs.h"
#include "libcore/rule.h"
#include "ruleset.h"

namespace rules {

class RulesetsRegistry {
  public:
    static RulesetsRegistry *instance();

    RulesetsRegistry();
    ~RulesetsRegistry();

    std::size_t num_rulesets() const;
    const Ruleset &get_ruleset(const std::string &slug) const;

  private:
    void populate_rulesets();

    std::unordered_map<std::string, Ruleset> m_rulesets{};
};

} // namespace rules

#endif // LIBRULES_RULESETS_REGISTRY_H
