#include "algos.h"
#include "cash_amount.h"
#include "line_item.h"
#include "shortcuts.h"

#include "catch.hpp"
#include <deque>

using Catch::Matchers::Message;
using core::CashAmount;
using core::CreditDebit;
using core::LineItem;

TEST_CASE("basic properties", "[LineItem]") {
    LineItem item{CreditDebit::DEBIT, C(50)};

    SECTION("accessors") {
        REQUIRE(item.credit_debit() == CreditDebit::DEBIT);
        REQUIRE(item.amount() == C(50));
    }
}

TEST_CASE("default constructor", "[LineItem]") {
    LineItem item{};

    SECTION("accessors") {
        REQUIRE(item.credit_debit() == CreditDebit::CREDIT);
        REQUIRE(item.amount() == C(0));
    }
}

TEST_CASE("special member functions", "[LineItem]") {
    SECTION("copy constructor") {
        LineItem item{CreditDebit::DEBIT, C(50)};
        LineItem other{item};
        REQUIRE(other.credit_debit() == CreditDebit::DEBIT);
        REQUIRE(other.amount() == C(50));
    }

    SECTION("copy assignment") {
        LineItem item{CreditDebit::DEBIT, C(50)};
        LineItem other = item;
        REQUIRE(other.credit_debit() == CreditDebit::DEBIT);
        REQUIRE(other.amount() == C(50));
    }

    SECTION("move constructor") {
        LineItem item{CreditDebit::DEBIT, C(50)};
        LineItem other{std::move(item)};
        REQUIRE(other.credit_debit() == CreditDebit::DEBIT);
        REQUIRE(other.amount() == C(50));
    }

    SECTION("move assignment") {
        LineItem item{CreditDebit::DEBIT, C(50)};
        LineItem other = std::move(item);
        REQUIRE(other.credit_debit() == CreditDebit::DEBIT);
        REQUIRE(other.amount() == C(50));
    }
}

TEST_CASE("addition", "[LineItem]") {
    SECTION("same sign, credit") {
        LineItem left{CreditDebit::CREDIT, C(50)};
        LineItem right{CreditDebit::CREDIT, C(70)};

        auto res = left + right;

        REQUIRE(res.credit_debit() == CreditDebit::CREDIT);
        REQUIRE(res.amount() == C(120));
    }

    SECTION("same sign, debit") {
        LineItem left{CreditDebit::DEBIT, C(50)};
        LineItem right{CreditDebit::DEBIT, C(70)};

        auto res = left + right;

        REQUIRE(res.credit_debit() == CreditDebit::DEBIT);
        REQUIRE(res.amount() == C(120));
    }

    SECTION("different sign, credit") {
        LineItem left{CreditDebit::DEBIT, C(50)};
        LineItem right{CreditDebit::CREDIT, C(70)};

        auto res = left + right;

        REQUIRE(res.credit_debit() == CreditDebit::CREDIT);
        REQUIRE(res.amount() == C(20));
    }

    SECTION("different sign, credit") {
        LineItem left{CreditDebit::DEBIT, C(70)};
        LineItem right{CreditDebit::CREDIT, C(50)};

        auto res = left + right;

        REQUIRE(res.credit_debit() == CreditDebit::DEBIT);
        REQUIRE(res.amount() == C(20));
    }
}

TEST_CASE("relational operators", "[LineItem]") {
    SECTION("equality operator") {
        LineItem ref{CreditDebit::DEBIT, C(50)};
        LineItem same{CreditDebit::DEBIT, C(50)};
        LineItem different{CreditDebit::CREDIT, C(50)};

        REQUIRE(ref == ref);
        REQUIRE(ref == same);
        REQUIRE(ref != different);
    }
}

TEST_CASE("algorithms", "[LineItem]") {
    LineItem first{CreditDebit::CREDIT, C(50)};
    LineItem second{CreditDebit::DEBIT, C(70)};
    LineItem third{CreditDebit::CREDIT, C(30)};

    SECTION("sum") {
        std::deque<LineItem> items{first, second, third};
        LineItem res = sum(items.begin(), items.end(), LineItem{});
        REQUIRE(res.credit_debit() == CreditDebit::CREDIT);
        REQUIRE(res.amount() == C(10));
    }
}