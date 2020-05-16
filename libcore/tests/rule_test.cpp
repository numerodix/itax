#include "rule.h"
#include "shortcuts.h"

#include "catch.hpp"

using Catch::Matchers::Message;
using core::CashAmount;
using core::CreditDebit;
using core::IncomeSlice;
using core::LineItem;
using core::Rule;
using core::RuleId;
using core::RuleItems;
using core::TaxReturn;

TEST_CASE("basic properties", "[Rule]") {
    auto fn = FN_CALC_SIGNATURE {
        LineItem first{C(10), C(5), CreditDebit::DEBIT};
        LineItem second{C(10), C(3), CreditDebit::CREDIT};
        return {first, second};
    };

    Rule rule{7, "slug", "desc", fn};

    SECTION("accessors") {
        REQUIRE(rule.rule_id() == 7);
        REQUIRE(rule.slug() == "slug");
        REQUIRE(rule.desc() == "desc");
    }

    SECTION("calculate") {
        TaxReturn taxret{{}};
        IncomeSlice slice{};

        RuleItems rule_items = rule.calculate(taxret, slice);

        REQUIRE(rule_items.rule_id() == 7);
        REQUIRE(rule_items.net().taxable() == C(20));
        REQUIRE(rule_items.net().payable() == C(2));
        REQUIRE(rule_items.net().credit_debit() == CreditDebit::DEBIT);
    }
}

TEST_CASE("special member functions", "[Rule]") {
    auto fn = FN_CALC_SIGNATURE {
        LineItem item{C(10), C(5), CreditDebit::DEBIT};
        return {item};
    };
    Rule rule{7, "slug", "desc", fn};

    TaxReturn taxret{{}};
    IncomeSlice slice{};

    SECTION("copy constructor") {
        Rule other{rule};
        REQUIRE(other.rule_id() == 7);
        REQUIRE(other.slug() == "slug");
        REQUIRE(other.desc() == "desc");

        auto rule_items = other.calculate(taxret, slice);

        REQUIRE(rule_items.rule_id() == 7);
        REQUIRE(rule_items.net().taxable() == C(10));
        REQUIRE(rule_items.net().payable() == C(5));
        REQUIRE(rule_items.net().credit_debit() == CreditDebit::DEBIT);
    }

    SECTION("copy assignment") {
        Rule other = rule;
        REQUIRE(other.rule_id() == 7);
        REQUIRE(other.slug() == "slug");
        REQUIRE(other.desc() == "desc");

        auto rule_items = other.calculate(taxret, slice);

        REQUIRE(rule_items.rule_id() == 7);
        REQUIRE(rule_items.net().taxable() == C(10));
        REQUIRE(rule_items.net().payable() == C(5));
        REQUIRE(rule_items.net().credit_debit() == CreditDebit::DEBIT);
    }

    SECTION("move constructor") {
        Rule other{std::move(rule)};
        REQUIRE(other.rule_id() == 7);
        REQUIRE(other.slug() == "slug");
        REQUIRE(other.desc() == "desc");

        auto rule_items = other.calculate(taxret, slice);

        REQUIRE(rule_items.rule_id() == 7);
        REQUIRE(rule_items.net().taxable() == C(10));
        REQUIRE(rule_items.net().payable() == C(5));
        REQUIRE(rule_items.net().credit_debit() == CreditDebit::DEBIT);
    }

    SECTION("move assignment") {
        Rule other = std::move(rule);
        REQUIRE(other.rule_id() == 7);
        REQUIRE(other.slug() == "slug");
        REQUIRE(other.desc() == "desc");

        auto rule_items = other.calculate(taxret, slice);

        REQUIRE(rule_items.rule_id() == 7);
        REQUIRE(rule_items.net().taxable() == C(10));
        REQUIRE(rule_items.net().payable() == C(5));
        REQUIRE(rule_items.net().credit_debit() == CreditDebit::DEBIT);
    }
}