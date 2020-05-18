#include "libcore/rule.h"
#include "libcore/shortcuts.h"
#include "ruleset.h"

#include "catch.hpp"

using Catch::Detail::Approx;
using core::CashAmount;
using core::CreditDebit;
using core::IncomeSlice;
using core::LineItem;
using core::Rule;
using core::TaxReturn;
using rules::Ruleset;
using rules::TaxCalculation;

TEST_CASE("basic properties", "[Ruleset]") {
    auto fn1 = FN_CALC_SIGNATURE { return {}; };
    Rule rule1{7, "slug-1", "desc-1", fn1};

    auto fn2 = FN_CALC_SIGNATURE { return {}; };
    Rule rule2{9, "slug-2", "desc-2", fn2};

    Ruleset set{"slug", "desc", {rule1, rule2}};

    SECTION("accessors") {
        REQUIRE(set.slug() == "slug");
        REQUIRE(set.desc() == "desc");
        REQUIRE(set.num_rules() == 2);
    }
}

TEST_CASE("apply", "[Ruleset]") {
    auto fn1 = FN_CALC_SIGNATURE {
        LineItem first{C(10000), C(1000), CreditDebit::CREDIT};
        LineItem second{C(20000), C(4000), CreditDebit::DEBIT};
        return {first, second};
    };
    Rule rule1{7, "slug-1", "desc-1", fn1};

    auto fn2 = FN_CALC_SIGNATURE {
        LineItem first{C(30000), C(15000), CreditDebit::DEBIT};
        return {first};
    };
    Rule rule2{9, "slug-2", "desc-2", fn2};

    Ruleset set{"slug", "desc", {rule1, rule2}};

    SECTION("apply to tax return with single slice") {
        IncomeSlice slice_input{C(0), C(50000)};
        TaxReturn taxret{{slice_input}};

        TaxCalculation calc = set.apply(taxret);

        // Get the slices out
        REQUIRE(calc.num_slices() == 1);

        auto slices = calc.slices();
        REQUIRE(slices.size() == 1);

        // Slice #1
        {
            auto slice = slices[0];
            REQUIRE(slice == slice_input);

            // Get the RuleItems out
            auto rule_items = calc.get_ruleitems(slice);

            // RuleItem #1 (bracket #1)
            auto first = rule_items[0];
            REQUIRE(first.rule_id() == 7);
            REQUIRE(first.net().taxable() == C(30000));
            REQUIRE(first.net().payable() == C(3000));
            REQUIRE(first.net().credit_debit() == CreditDebit::DEBIT);
            REQUIRE(first.net().percent() == 0.1);
            REQUIRE(first.net().after_tax() == C(27000));

            // RuleItem #2 (bracket #2)
            auto second = rule_items[1];
            REQUIRE(second.rule_id() == 9);
            REQUIRE(second.net().taxable() == C(30000));
            REQUIRE(second.net().payable() == C(15000));
            REQUIRE(second.net().credit_debit() == CreditDebit::DEBIT);
            REQUIRE(second.net().percent() == 0.5);
            REQUIRE(second.net().after_tax() == C(15000));

            // Total the slice
            auto net_item = calc.slice_total(slice);
            REQUIRE(net_item.taxable() == C(50000));
            REQUIRE(net_item.payable() == C(18000));
            REQUIRE(net_item.credit_debit() == CreditDebit::DEBIT);
            REQUIRE(net_item.percent() == 0.36);
            REQUIRE(net_item.after_tax() == C(32000));
        }

        // Net the slices

        auto net_slices = calc.net_slices();
        REQUIRE(net_slices.size() == 2);

        auto net_slice_first = net_slices[0];
        REQUIRE(net_slice_first.rule_id() == 7);
        REQUIRE(net_slice_first.net().taxable() == C(30000));
        REQUIRE(net_slice_first.net().payable() == C(3000));
        REQUIRE(net_slice_first.net().credit_debit() == CreditDebit::DEBIT);
        REQUIRE(net_slice_first.net().percent() == 0.1);
        REQUIRE(net_slice_first.net().after_tax() == C(27000));

        auto net_slice_second = net_slices[1];
        REQUIRE(net_slice_second.rule_id() == 9);
        REQUIRE(net_slice_second.net().taxable() == C(30000));
        REQUIRE(net_slice_second.net().payable() == C(15000));
        REQUIRE(net_slice_second.net().credit_debit() == CreditDebit::DEBIT);
        REQUIRE(net_slice_second.net().percent() == 0.5);
        REQUIRE(net_slice_second.net().after_tax() == C(15000));

        // Total the return

        auto net_return = calc.return_total();
        REQUIRE(net_return.taxable() == C(50000));
        REQUIRE(net_return.payable() == C(18000));
        REQUIRE(net_return.credit_debit() == CreditDebit::DEBIT);
        REQUIRE(net_return.percent() == 0.36);
        REQUIRE(net_return.after_tax() == C(32000));
    }

    SECTION("apply to tax return with two slices") {
        IncomeSlice slice_input_first{C(0), C(50000)};
        IncomeSlice slice_input_second{C(50000), C(20000)};
        TaxReturn taxret{{slice_input_first, slice_input_second}};

        TaxCalculation calc = set.apply(taxret);

        // Get the slices out
        REQUIRE(calc.num_slices() == 2);

        auto slices = calc.slices();
        REQUIRE(slices.size() == 2);

        // Slice #1
        {
            auto slice = slices[0];
            REQUIRE(slice == slice_input_first);

            // Get the RuleItems out
            auto rule_items = calc.get_ruleitems(slice);

            // RuleItem #1 (bracket #1)
            auto first = rule_items[0];
            REQUIRE(first.rule_id() == 7);
            REQUIRE(first.net().taxable() == C(30000));
            REQUIRE(first.net().payable() == C(3000));
            REQUIRE(first.net().credit_debit() == CreditDebit::DEBIT);
            REQUIRE(first.net().percent() == 0.1);
            REQUIRE(first.net().after_tax() == C(27000));

            // RuleItem #2 (bracket #2)
            auto second = rule_items[1];
            REQUIRE(second.rule_id() == 9);
            REQUIRE(second.net().taxable() == C(30000));
            REQUIRE(second.net().payable() == C(15000));
            REQUIRE(second.net().credit_debit() == CreditDebit::DEBIT);
            REQUIRE(second.net().percent() == 0.5);
            REQUIRE(second.net().after_tax() == C(15000));

            // Total the slice
            auto net_item = calc.slice_total(slice);
            REQUIRE(net_item.taxable() == C(50000));
            REQUIRE(net_item.payable() == C(18000));
            REQUIRE(net_item.credit_debit() == CreditDebit::DEBIT);
            REQUIRE(net_item.percent() == 0.36);
            REQUIRE(net_item.after_tax() == C(32000));
        }

        // Slice #2
        {
            auto slice = slices[1];
            REQUIRE(slice == slice_input_second);

            // Get the RuleItems out
            auto rule_items = calc.get_ruleitems(slice);

            // RuleItem #1 (bracket #1)
            auto first = rule_items[0];
            REQUIRE(first.rule_id() == 7);
            REQUIRE(first.net().taxable() == C(30000));
            REQUIRE(first.net().payable() == C(3000));
            REQUIRE(first.net().credit_debit() == CreditDebit::DEBIT);
            REQUIRE(first.net().percent() == 0.1);
            REQUIRE(first.net().after_tax() == C(27000));

            // RuleItem #2 (bracket #2)
            auto second = rule_items[1];
            REQUIRE(second.rule_id() == 9);
            REQUIRE(second.net().taxable() == C(30000));
            REQUIRE(second.net().payable() == C(15000));
            REQUIRE(second.net().credit_debit() == CreditDebit::DEBIT);
            REQUIRE(second.net().percent() == 0.5);
            REQUIRE(second.net().after_tax() == C(15000));

            // Total the slice
            auto net_item = calc.slice_total(slice);
            REQUIRE(net_item.taxable() == C(20000));
            REQUIRE(net_item.payable() == C(18000));
            REQUIRE(net_item.credit_debit() == CreditDebit::DEBIT);
            REQUIRE(net_item.percent() == 0.9);
            REQUIRE(net_item.after_tax() == C(2000));
        }

        // Net the slices

        auto net_slices = calc.net_slices();
        REQUIRE(net_slices.size() == 2);

        auto net_slice_first = net_slices[0];
        REQUIRE(net_slice_first.rule_id() == 7);
        REQUIRE(net_slice_first.net().taxable() == C(60000));
        REQUIRE(net_slice_first.net().payable() == C(6000));
        REQUIRE(net_slice_first.net().credit_debit() == CreditDebit::DEBIT);
        REQUIRE(net_slice_first.net().percent() == 0.1);
        REQUIRE(net_slice_first.net().after_tax() == C(54000));

        auto net_slice_second = net_slices[1];
        REQUIRE(net_slice_second.rule_id() == 9);
        REQUIRE(net_slice_second.net().taxable() == C(60000));
        REQUIRE(net_slice_second.net().payable() == C(30000));
        REQUIRE(net_slice_second.net().credit_debit() == CreditDebit::DEBIT);
        REQUIRE(net_slice_second.net().percent() == 0.5);
        REQUIRE(net_slice_second.net().after_tax() == C(30000));

        // Total the return

        auto net_return = calc.return_total();
        REQUIRE(net_return.taxable() == C(70000));
        REQUIRE(net_return.payable() == C(36000));
        REQUIRE(net_return.credit_debit() == CreditDebit::DEBIT);
        REQUIRE(net_return.percent() == Approx(0.514).epsilon(0.001));
        REQUIRE(net_return.after_tax() == C(34000));
    }
}