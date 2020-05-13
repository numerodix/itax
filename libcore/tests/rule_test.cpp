#include "cash_amount.h"
#include "line_item.h"
#include "rule.h"
#include "shortcuts.h"
#include "tax_return.h"

#include "catch.hpp"

using Catch::Matchers::Message;
using core::CashAmount;
using core::CreditDebit;
using core::FnCalcForSlice;
using core::IncomeSlice;
using core::LineItem;
using core::Rule;
using core::RuleId;
using core::TaxReturn;

TEST_CASE("flat tax rule", "[Rule]") {

    // Rule

    RuleId rule_id = 1;
    std::string slug{"some-levy-3%"};
    std::string desc{"Some levy is 3% on all income"};

    FnCalcForSlice fn = [](const TaxReturn &taxret,
                           const IncomeSlice &slice) -> std::vector<LineItem> {
        CashAmount debit = slice.amount() * 0.03;
        LineItem item{CreditDebit::DEBIT, debit};
        return {{item}};
    };

    Rule rule{rule_id, slug, desc, fn};

    // Income

    IncomeSlice first{C(0), C(20000)};
    IncomeSlice second{C(20000), C(10000)};

    TaxReturn taxret{{{first, second}}};

    auto res = rule.calculate_for_slice(taxret, second);

    REQUIRE(res.rule_id() == rule_id);
    REQUIRE(res.items().size() == 1);
    REQUIRE(res.items()[0].credit_debit() == CreditDebit::DEBIT);
    REQUIRE(res.items()[0].amount() == C(300));
}

TEST_CASE("rule takes effect above a threshold", "[Rule]") {

    // Rule

    RuleId rule_id = 1;
    std::string slug{"special-levy-1%"};
    std::string desc{"Special levy is 1% on all income if total income above 50k"};

    FnCalcForSlice fn = [](const TaxReturn &taxret,
                           const IncomeSlice &slice) -> std::vector<LineItem> {

        if (taxret.total_income() > C(50000)) {
            CashAmount debit = slice.amount() * 0.01;
            LineItem item{CreditDebit::DEBIT, debit};
            return {{item}};
        }

        return {{}};
    };

    Rule rule{rule_id, slug, desc, fn};

    // Income

    IncomeSlice first{C(0), C(30000)};
    IncomeSlice second{C(30000), C(70000)};

    TaxReturn taxret{{{first, second}}};

    auto res = rule.calculate_for_slice(taxret, second);

    REQUIRE(res.rule_id() == rule_id);
    REQUIRE(res.items().size() == 1);
    REQUIRE(res.items()[0].credit_debit() == CreditDebit::DEBIT);
    REQUIRE(res.items()[0].amount() == C(700));
}

TEST_CASE("bracket tax rule", "[Rule]") {

    // Rule

    RuleId rule_id = 1;
    std::string slug{"bracket-2"};
    std::string desc{"15% on income 20k - 31k"};

    FnCalcForSlice fn = [](const TaxReturn &taxret,
                           const IncomeSlice &slice) -> std::vector<LineItem> {

        std::vector<LineItem> items{};

        auto bracket_lower = C(20000);
        auto bracket_upper = C(31000);
        CashAmount in_bracket{0L};

        if (slice.upper_bound() > bracket_upper) {
            in_bracket = bracket_upper - bracket_lower;
        } else if (slice.upper_bound() > bracket_lower) {
            in_bracket = slice.upper_bound() - bracket_lower;
        }

        if (in_bracket > C(0)) {
            CashAmount debit = in_bracket * 0.15;
            LineItem item{CreditDebit::DEBIT, debit};
            return {{item}};
        }

        return {{}};
    };

    Rule rule{rule_id, slug, desc, fn};

    // Income

    IncomeSlice first{C(0), C(18000)};
    IncomeSlice second{C(18000), C(8000)};

    TaxReturn taxret{{{first, second}}};

    auto res = rule.calculate_for_slice(taxret, second);

    REQUIRE(res.rule_id() == rule_id);
    REQUIRE(res.items().size() == 1);
    REQUIRE(res.items()[0].credit_debit() == CreditDebit::DEBIT);
    REQUIRE(res.items()[0].amount() == C(900));
}
