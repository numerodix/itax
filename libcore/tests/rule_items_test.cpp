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
    LineItem first{C(100), C(50), CreditDebit::DEBIT};
    LineItem second{C(100), C(30), CreditDebit::CREDIT};
    std::vector<LineItem> items = {first, second};

    RuleItems rule_items{1, items};

    SECTION("accessors") {
        REQUIRE(rule_items.rule_id() == 1);
        REQUIRE(rule_items.items()[0] == first);
        REQUIRE(rule_items.items()[1] == second);
    }

    SECTION("net") {
        LineItem expected{C(100), C(20), CreditDebit::DEBIT};
        REQUIRE(rule_items.net() == expected);
    }

    SECTION("append_item") {
        LineItem item{C(200), C(50), CreditDebit::DEBIT};
        rule_items.append_item(item);

        LineItem expected{C(300), C(70), CreditDebit::DEBIT};
        REQUIRE(rule_items.net() == expected);
    }
}

TEST_CASE("special member functions", "[RuleItems]") {
    LineItem first{C(100), C(50), CreditDebit::DEBIT};
    LineItem second{C(100), C(30), CreditDebit::CREDIT};
    std::vector<LineItem> items = {first, second};

    RuleItems rule_items{1, items};

    SECTION("copy constructor") {
        RuleItems other{rule_items};
        REQUIRE(other.rule_id() == 1);
        REQUIRE(other.items()[0] == first);
        REQUIRE(other.items()[1] == second);
    }

    SECTION("copy assignment") {
        RuleItems other = rule_items;
        REQUIRE(other.rule_id() == 1);
        REQUIRE(other.items()[0] == first);
        REQUIRE(other.items()[1] == second);
    }

    SECTION("move constructor") {
        RuleItems other{std::move(rule_items)};
        REQUIRE(other.rule_id() == 1);
        REQUIRE(other.items()[0] == first);
        REQUIRE(other.items()[1] == second);
    }

    SECTION("move assignment") {
        RuleItems other = std::move(rule_items);
        REQUIRE(other.rule_id() == 1);
        REQUIRE(other.items()[0] == first);
        REQUIRE(other.items()[1] == second);
    }
}