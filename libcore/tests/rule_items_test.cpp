#include "cash_amount.h"
#include "line_item.h"
#include "rule_items.h"
#include "shortcuts.h"

#include "catch.hpp"

using core::CashAmount;
using core::CreditDebit;
using core::LineItem;
using core::RuleItems;

TEST_CASE("basic properties", "[RuleItems]") {
    LineItem first{CreditDebit::DEBIT, C(50)};
    LineItem second{CreditDebit::CREDIT, C(30)};
    std::vector<LineItem> items = {first, second};

    RuleItems rule_items{1, items};

    SECTION("accessors") {
        REQUIRE(rule_items.rule_id() == 1);
        REQUIRE(rule_items.items()[0] == first);
        REQUIRE(rule_items.items()[1] == second);
    }

    SECTION("net") {
        LineItem expected{CreditDebit::DEBIT, C(20)};
        REQUIRE(rule_items.net() == expected);
    }
}