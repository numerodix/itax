#include "collections/aus.h"
#include "libcore/cash_amount.h"
#include "libcore/income_slice.h"
#include "libcore/rule_items.h"
#include "libcore/shortcuts.h"
#include "libcore/tax_return.h"

#include "catch.hpp"

using namespace core;
using namespace rules::collections;

TEST_CASE("Low and Middle Income Tax Offset constant bracket",
          "[aus_fy19_lmito]") {
    auto rule = get_aus_rev_fy19_lmito();

    SECTION("each slice exactly matches each bracket") {
        IncomeSlice first{C(0), C(37000)};
        IncomeSlice second{C(37000), C(11000)};
        IncomeSlice third{C(48000), C(42000)};
        IncomeSlice fourth{C(90000), C(36000)};
        TaxReturn taxret{{first, second, third, fourth}};

        RuleItems fst = rule.calculate(taxret, first);
        REQUIRE(fst.net().taxable() == C(37000));
        REQUIRE(fst.net().payable() == C(255));
        REQUIRE(fst.net().credit_debit() == CreditDebit::CREDIT);

        RuleItems snd = rule.calculate(taxret, second);
        REQUIRE(snd.net().taxable() == C(11000));
        REQUIRE(snd.net().payable() == C(825));
        REQUIRE(snd.net().credit_debit() == CreditDebit::CREDIT);

        RuleItems thd = rule.calculate(taxret, third);
        REQUIRE(thd.net().taxable() == C(42000));
        REQUIRE(thd.net().payable() == C(0));
        REQUIRE(thd.net().credit_debit() == CreditDebit::CREDIT);

        RuleItems fth = rule.calculate(taxret, fourth);
        REQUIRE(fth.net().taxable() == C(36000));
        REQUIRE(fth.net().payable() == C(1080));
        REQUIRE(fth.net().credit_debit() == CreditDebit::DEBIT);
    }

    SECTION("five slices") {
        IncomeSlice first{C(0), C(30000)};
        IncomeSlice second{C(30000), C(15000)};
        IncomeSlice third{C(45000), C(15000)};
        IncomeSlice fourth{C(60000), C(60000)};
        IncomeSlice fifth{C(120000), C(30000)};
        TaxReturn taxret{{first, second, third, fourth, fifth}};

        RuleItems fst = rule.calculate(taxret, first);
        REQUIRE(fst.net().taxable() == C(30000));
        REQUIRE(fst.net().payable() == CashAmount{2067567L}); // 206.7567
        REQUIRE(fst.net().credit_debit() == CreditDebit::CREDIT);

        RuleItems snd = rule.calculate(taxret, second);
        REQUIRE(snd.net().taxable() == C(15000));
        REQUIRE(snd.net().payable() == CashAmount{6482432L}); // 48.2432 + 600
        REQUIRE(snd.net().credit_debit() == CreditDebit::CREDIT);

        RuleItems thd = rule.calculate(taxret, third);
        REQUIRE(thd.net().taxable() == C(15000));
        REQUIRE(thd.net().payable() == C(225));
        REQUIRE(thd.net().credit_debit() == CreditDebit::CREDIT);

        RuleItems foth = rule.calculate(taxret, fourth);
        REQUIRE(foth.net().taxable() == C(60000));
        REQUIRE(foth.net().payable() == C(900));
        REQUIRE(foth.net().credit_debit() == CreditDebit::DEBIT);

        RuleItems fith = rule.calculate(taxret, fifth);
        REQUIRE(fith.net().taxable() == C(6000));
        REQUIRE(fith.net().payable() == C(180));
        REQUIRE(fith.net().credit_debit() == CreditDebit::DEBIT);

        LineItem sum =
            fst.net() + snd.net() + thd.net() + foth.net() + fith.net();
        // FIXME: should be 0
        REQUIRE(sum.payable() == CashAmount{1L});
        REQUIRE(sum.credit_debit() == CreditDebit::DEBIT);
    }
}