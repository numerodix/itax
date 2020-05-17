#include "libcore/rule.h"
#include "libcore/shortcuts.h"
#include "ruleset.h"

#include "catch.hpp"

using core::CashAmount;
using core::CreditDebit;
using core::IncomeSlice;
using core::LineItem;
using core::Rule;
using core::TaxReturn;
using rules::RuleSet;
using rules::TaxCalculation;

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
        REQUIRE(set.get_rule(7).rule_id() == 7);
        REQUIRE(set.get_rule(7).slug() == "slug-1");
        REQUIRE(set.get_rule(7).desc() == "desc-1");

        REQUIRE(set.get_rule(9).rule_id() == 9);
        REQUIRE(set.get_rule(9).slug() == "slug-2");
        REQUIRE(set.get_rule(9).desc() == "desc-2");
    }
}

TEST_CASE("apply", "[RuleSet]") {
    auto fn1 = FN_CALC_SIGNATURE {
        LineItem first{C(2000), C(1400), CreditDebit::DEBIT};
        return {first};
    };
    Rule rule1{7, "slug-1", "desc-1", fn1};

    auto fn2 = FN_CALC_SIGNATURE {
        LineItem first{C(1000), C(500), CreditDebit::DEBIT};
        LineItem second{C(100), C(30), CreditDebit::CREDIT};
        return {first, second};
    };
    Rule rule2{9, "slug-2", "desc-2", fn2};

    RuleSet set{"slug", "desc", {rule1, rule2}};

    SECTION("get_rule") {
        REQUIRE(set.get_rule(7).rule_id() == 7);
        REQUIRE(set.get_rule(7).slug() == "slug-1");
        REQUIRE(set.get_rule(7).desc() == "desc-1");

        REQUIRE(set.get_rule(9).rule_id() == 9);
        REQUIRE(set.get_rule(9).slug() == "slug-2");
        REQUIRE(set.get_rule(9).desc() == "desc-2");
    }

    SECTION("apply to tax return with single slice") {
        IncomeSlice slice_input{C(0), C(5000)};
        TaxReturn taxret{{slice_input}};

        TaxCalculation calc = set.apply(taxret);

        // Get the slices out
        REQUIRE(calc.num_slices() == 1);

        auto slices = calc.slices();
        REQUIRE(slices.size() == 1);

        // Slice #1

        auto slice = slices[0];
        REQUIRE(slice == slice_input);

        // Get the RuleItems out
        auto rule_items = calc.get_ruleitems(slice);

        // RuleItem #1
        auto first = rule_items[0];
        REQUIRE(first.rule_id() == 7);
        REQUIRE(first.net().taxable() == C(2000));
        REQUIRE(first.net().payable() == C(1400));
        REQUIRE(first.net().credit_debit() == CreditDebit::DEBIT);
        REQUIRE(first.net().after_tax() == C(600));

        // RuleItem #2
        auto second = rule_items[1];
        REQUIRE(second.rule_id() == 9);
        REQUIRE(second.net().taxable() == C(1100));
        REQUIRE(second.net().payable() == C(470));
        REQUIRE(second.net().credit_debit() == CreditDebit::DEBIT);
        REQUIRE(second.net().after_tax() == C(630));
    }
}