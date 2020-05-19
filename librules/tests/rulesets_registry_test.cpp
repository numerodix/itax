#include "rulesets_registry.h"

#include "catch.hpp"

using rules::RulesetsRegistry;

TEST_CASE("spot check registry", "[RulesetsRegistry]") {
    auto registry = RulesetsRegistry::instance();

    REQUIRE(registry->num_rulesets() > 0);
    auto ruleset = registry->get_ruleset("aus-2020");

    REQUIRE(ruleset.num_rules() > 0);
    REQUIRE(ruleset.slug() == "aus-2020");
    REQUIRE(ruleset.desc() == "Australian Income Tax 2020");
}
