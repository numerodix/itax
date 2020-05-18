#include "collections/aus.h"
#include "libcore/rule.h"
#include "libcore/shortcuts.h"
#include "rules_registry.h"

#include "catch.hpp"

using core::CashAmount;
using core::IncomeSlice;
using core::TaxReturn;
using rules::RulesRegistry;
using namespace rules::collections;

TEST_CASE("spot check registry", "[RulesRegistry]") {
    auto registry = RulesRegistry::instance();

    REQUIRE(registry->num_rules() > 0);
    auto rule = registry->get_rule(AUS_REV_FY19_BRACKET2);

    IncomeSlice slice{C(0), C(50000)};
    TaxReturn taxret{{}};

    auto rule_items = rule.calculate(taxret, slice);
    REQUIRE(rule_items.net().taxable() == C(18800));
    REQUIRE(rule_items.net().payable() == C(3572));
    REQUIRE(rule_items.net().percent() == 0.19);
}
