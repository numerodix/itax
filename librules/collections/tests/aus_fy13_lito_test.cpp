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
    auto rule = get_aus_rev_fy13_lito();

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

    SECTION("two slices, total income above bracket end") {
        IncomeSlice first{C(0), C(10000)};
        IncomeSlice second{C(10000), C(15000)};
        TaxReturn taxret{{first, second}};

        RuleItems ri_first = rule.calculate(taxret, first);
        REQUIRE(ri_first.net().taxable() == C(10000));
        REQUIRE(ri_first.net().payable() == C(178));
        REQUIRE(ri_first.net().credit_debit() == CreditDebit::CREDIT);

        RuleItems ri_second = rule.calculate(taxret, second);
        REQUIRE(ri_second.net().taxable() == C(15000));
        REQUIRE(ri_second.net().payable() == C(267));
        REQUIRE(ri_second.net().credit_debit() == CreditDebit::CREDIT);
    }
}

TEST_CASE("Low Income Tax Offset phaseout bracket", "[aus_fy13_lito]") {
    auto rule = get_aus_rev_fy13_lito();

    SECTION("single slice, total income below bracket end") {
        IncomeSlice first{C(0), C(37000)};
        IncomeSlice second{C(37000), C(14000)};
        TaxReturn taxret{{first, second}};

        RuleItems rule_items = rule.calculate(taxret, second);
        REQUIRE(rule_items.net().taxable() == C(14000));
        REQUIRE(rule_items.net().payable() == CashAmount{2099976L}); // 209.99
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

        RuleItems ri_fst = rule.calculate(taxret, first);
        REQUIRE(ri_fst.net().taxable() == C(30000));
        REQUIRE(ri_fst.net().payable() == CashAmount{3608108L}); // 360.81
        REQUIRE(ri_fst.net().credit_debit() == CreditDebit::CREDIT);

        RuleItems ri_snd = rule.calculate(taxret, second);
        REQUIRE(ri_snd.net().taxable() == C(10000));
        REQUIRE(ri_snd.net().payable() == CashAmount{391897L}); // 84.1897 - 45
        REQUIRE(ri_snd.net().credit_debit() == CreditDebit::CREDIT);

        RuleItems ri_thd = rule.calculate(taxret, third);
        REQUIRE(ri_thd.net().taxable() == C(15000));
        REQUIRE(ri_thd.net().payable() == CashAmount{2249974L}); // 224.99
        REQUIRE(ri_thd.net().credit_debit() == CreditDebit::DEBIT);

        RuleItems ri_fth = rule.calculate(taxret, fourth);
        REQUIRE(ri_fth.net().taxable() == C(11667));
        REQUIRE(ri_fth.net().payable() == CashAmount{1750030L}); // 175.003
        REQUIRE(ri_fth.net().credit_debit() == CreditDebit::DEBIT);

        LineItem sum =
            ri_fst.net() + ri_snd.net() + ri_thd.net() + ri_fth.net();
        // FIXME: this should be 0
        REQUIRE(sum.payable() == CashAmount{1L});
        REQUIRE(sum.credit_debit() == CreditDebit::CREDIT);
    }
}