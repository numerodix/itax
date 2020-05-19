#include "collections/aus.h"
#include "libcore/cash_amount.h"
#include "libcore/income_slice.h"
#include "libcore/rule_items.h"
#include "libcore/shortcuts.h"
#include "libcore/tax_return.h"

#include "catch.hpp"

using namespace core;
using namespace rules::collections;

TEST_CASE("Low Income Tax Offset constant bracket", "[aus_fy13_lito]") {
    auto rule = get_aus_rev_fy13_lito_bracket1();

    SECTION("single slice, total income below bracket end") {
        IncomeSlice slice{C(0), C(30000)};
        TaxReturn taxret{{slice}};

        RuleItems rule_items = rule.calculate(taxret, slice);
        REQUIRE(rule_items.net().taxable() == C(30000));
        REQUIRE(rule_items.net().payable() == C(445));
        REQUIRE(rule_items.net().credit_debit() == CreditDebit::CREDIT);
    }

    SECTION("single slice, exact overlap") {
        IncomeSlice slice{C(0), C(37000)};
        TaxReturn taxret{{slice}};

        RuleItems rule_items = rule.calculate(taxret, slice);
        REQUIRE(rule_items.net().taxable() == C(37000));
        REQUIRE(rule_items.net().payable() == C(445));
        REQUIRE(rule_items.net().credit_debit() == CreditDebit::CREDIT);
    }

    SECTION("single slice, total income above bracket end") {
        IncomeSlice slice{C(0), C(50000)};
        TaxReturn taxret{{slice}};

        RuleItems rule_items = rule.calculate(taxret, slice);
        REQUIRE(rule_items.net().taxable() == C(37000));
        REQUIRE(rule_items.net().payable() == C(445));
        REQUIRE(rule_items.net().credit_debit() == CreditDebit::CREDIT);
    }

    SECTION("three slices, total income above bracket end") {
        IncomeSlice first{C(0), C(10000)};
        IncomeSlice second{C(10000), C(15000)};
        IncomeSlice third{C(25000), C(20000)};
        TaxReturn taxret{{first, second, third}};

        RuleItems ri_first = rule.calculate(taxret, first);
        REQUIRE(ri_first.net().taxable() == C(10000));
        REQUIRE(ri_first.net().payable() == CashAmount{1202702L}); // 120.27
        REQUIRE(ri_first.net().credit_debit() == CreditDebit::CREDIT);

        RuleItems ri_second = rule.calculate(taxret, second);
        REQUIRE(ri_second.net().taxable() == C(15000));
        REQUIRE(ri_second.net().payable() == CashAmount{1804054L}); // 180.40
        REQUIRE(ri_second.net().credit_debit() == CreditDebit::CREDIT);

        RuleItems ri_third = rule.calculate(taxret, third);
        REQUIRE(ri_third.net().taxable() == C(12000));
        REQUIRE(ri_third.net().payable() == CashAmount{1443243L}); // 144.32
        REQUIRE(ri_third.net().credit_debit() == CreditDebit::CREDIT);
    }
}

TEST_CASE("Low Income Tax Offset phaseout bracket", "[aus_fy13_lito]") {
    auto rule = get_aus_rev_fy13_lito_bracket2();

    SECTION("single slice, total income below bracket end") {
        IncomeSlice first{C(0), C(37000)};
        IncomeSlice second{C(37000), C(14000)};
        TaxReturn taxret{{first, second}};

        RuleItems rule_items = rule.calculate(taxret, second);
        REQUIRE(rule_items.net().taxable() == C(14000));
        REQUIRE(rule_items.net().payable() == C(210));
        REQUIRE(rule_items.net().credit_debit() == CreditDebit::DEBIT);
    }

    SECTION("single slice, total income at bracket end") {
        IncomeSlice first{C(0), C(37000)};
        IncomeSlice second{C(37000), C(29667)};
        TaxReturn taxret{{first, second}};

        RuleItems rule_items = rule.calculate(taxret, second);
        REQUIRE(rule_items.net().taxable() == C(29667));
        REQUIRE(rule_items.net().payable() == C(445));
        REQUIRE(rule_items.net().credit_debit() == CreditDebit::DEBIT);
    }

    SECTION("single slice, total income above bracket end") {
        IncomeSlice first{C(0), C(37000)};
        IncomeSlice second{C(37000), C(40000)};
        TaxReturn taxret{{first, second}};

        RuleItems rule_items = rule.calculate(taxret, second);
        REQUIRE(rule_items.net().taxable() == C(29667));
        REQUIRE(rule_items.net().payable() == C(445));
        REQUIRE(rule_items.net().credit_debit() == CreditDebit::DEBIT);
    }

    SECTION("three slices, total income above bracket end") {
        IncomeSlice first{C(0), C(30000)};
        IncomeSlice second{C(30000), C(10000)};
        IncomeSlice third{C(40000), C(15000)};
        IncomeSlice fourth{C(55000), C(20000)};
        TaxReturn taxret{{first, second, third, fourth}};

        RuleItems ri_first = rule.calculate(taxret, first);
        REQUIRE(ri_first.net().taxable() == C(0));
        REQUIRE(ri_first.net().payable() == C(0));
        REQUIRE(ri_first.net().credit_debit() == CreditDebit::CREDIT);

        RuleItems ri_second = rule.calculate(taxret, second);
        REQUIRE(ri_second.net().taxable() == C(3000));
        REQUIRE(ri_second.net().payable() == C(45));
        REQUIRE(ri_second.net().credit_debit() == CreditDebit::DEBIT);

        RuleItems ri_third = rule.calculate(taxret, third);
        REQUIRE(ri_third.net().taxable() == C(15000));
        REQUIRE(ri_third.net().payable() == C(225));
        REQUIRE(ri_third.net().credit_debit() == CreditDebit::DEBIT);

        RuleItems ri_fourth = rule.calculate(taxret, fourth);
        REQUIRE(ri_fourth.net().taxable() == C(11667));
        REQUIRE(ri_fourth.net().payable() == CashAmount{1750050L}); // 175.005
        REQUIRE(ri_fourth.net().credit_debit() == CreditDebit::DEBIT);
    }
}