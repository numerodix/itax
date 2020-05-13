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