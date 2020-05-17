#include "libcore/rule.h"
#include "ruleset.h"

#include "catch.hpp"

using core::IncomeSlice;
using core::LineItem;
using core::Rule;
using core::TaxReturn;
using rules::RuleSet;

TEST_CASE("basic properties", "[RuleSet]") {
    auto fn1 = FN_CALC_SIGNATURE { return {}; };
    Rule rule1{7, "slug-1", "desc-1", fn1};

    auto fn2 = FN_CALC_SIGNATURE { return {}; };
    Rule rule2{9, "slug-2", "desc-2", fn2};

    RuleSet set{"slug", "desc", {rule1, rule2}};

    SECTION("accessors") {
        REQUIRE(set.slug() == "slug");
        REQUIRE(set.desc() == "desc");
        REQUIRE(set.num_rules() == 2);
    }

    SECTION("get_rule") {
        REQUIRE(set.get_rule(7).slug() == "slug-1");
        REQUIRE(set.get_rule(9).slug() == "slug-2");
    }
}
