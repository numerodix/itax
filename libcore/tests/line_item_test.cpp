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
    SECTION("aborts if payable greater than taxable") {
        REQUIRE_THROWS_MATCHES(
            (LineItem{C(50), C(51), CreditDebit::DEBIT}), std::out_of_range,
            Message("constructor called with a payable greater than the taxable"));
    }

    SECTION("accessors") {
        LineItem item{C(100), C(50), CreditDebit::DEBIT};
        REQUIRE(item.taxable() == C(100));
        REQUIRE(item.payable() == C(50));
        REQUIRE(item.credit_debit() == CreditDebit::DEBIT);
    }

    SECTION("percent") {
        LineItem item{C(100), C(53), CreditDebit::DEBIT};
        REQUIRE(item.percent() == 0.53);
    }

    SECTION("after_tax") {
        LineItem item1{C(100), C(24), CreditDebit::DEBIT};
        REQUIRE(item1.after_tax() == C(76));

        LineItem item2{C(100), C(12), CreditDebit::CREDIT};
        REQUIRE(item2.after_tax() == C(112));
    }
}

TEST_CASE("default constructor", "[LineItem]") {
    LineItem item{};

    SECTION("accessors") {
        REQUIRE(item.taxable() == C(0));
        REQUIRE(item.payable() == C(0));
        REQUIRE(item.credit_debit() == CreditDebit::CREDIT);
    }

    SECTION("percent") {
        REQUIRE(item.percent() == 0.0);
    }

    SECTION("after_tax") {
        REQUIRE(item.after_tax() == C(0));
    }
}

TEST_CASE("special member functions", "[LineItem]") {
    SECTION("copy constructor") {
        LineItem item{C(100), C(50), CreditDebit::DEBIT};
        LineItem other{item};
        REQUIRE(item.taxable() == C(100));
        REQUIRE(other.payable() == C(50));
        REQUIRE(other.credit_debit() == CreditDebit::DEBIT);
    }

    SECTION("copy assignment") {
        LineItem item{C(100), C(50), CreditDebit::DEBIT};
        LineItem other = item;
        REQUIRE(item.taxable() == C(100));
        REQUIRE(other.payable() == C(50));
        REQUIRE(other.credit_debit() == CreditDebit::DEBIT);
    }

    SECTION("move constructor") {
        LineItem item{C(100), C(50), CreditDebit::DEBIT};
        LineItem other{std::move(item)};
        REQUIRE(item.taxable() == C(100));
        REQUIRE(other.payable() == C(50));
        REQUIRE(other.credit_debit() == CreditDebit::DEBIT);
    }

    SECTION("move assignment") {
        LineItem item{C(100), C(50), CreditDebit::DEBIT};
        LineItem other = std::move(item);
        REQUIRE(item.taxable() == C(100));
        REQUIRE(other.payable() == C(50));
        REQUIRE(other.credit_debit() == CreditDebit::DEBIT);
    }
}

TEST_CASE("addition", "[LineItem]") {
    SECTION("same sign, credit") {
        LineItem left{C(100), C(50), CreditDebit::CREDIT};
        LineItem right{C(100), C(70), CreditDebit::CREDIT};

        auto res = left + right;

        REQUIRE(res.taxable() == C(200));
        REQUIRE(res.payable() == C(120));
        REQUIRE(res.credit_debit() == CreditDebit::CREDIT);
    }

    SECTION("same sign, debit") {
        LineItem left{C(100), C(50), CreditDebit::DEBIT};
        LineItem right{C(100), C(70), CreditDebit::DEBIT};

        auto res = left + right;

        REQUIRE(res.taxable() == C(200));
        REQUIRE(res.payable() == C(120));
        REQUIRE(res.credit_debit() == CreditDebit::DEBIT);
    }

    SECTION("different sign, credit") {
        LineItem left{C(100), C(50), CreditDebit::DEBIT};
        LineItem right{C(100), C(70), CreditDebit::CREDIT};

        auto res = left + right;

        REQUIRE(res.taxable() == C(200));
        REQUIRE(res.payable() == C(20));
        REQUIRE(res.credit_debit() == CreditDebit::CREDIT);
    }

    SECTION("different sign, credit") {
        LineItem left{C(100), C(70), CreditDebit::DEBIT};
        LineItem right{C(100), C(50), CreditDebit::CREDIT};

        auto res = left + right;

        REQUIRE(res.taxable() == C(200));
        REQUIRE(res.payable() == C(20));
        REQUIRE(res.credit_debit() == CreditDebit::DEBIT);
    }
}

TEST_CASE("relational operators", "[LineItem]") {
    SECTION("equality operator") {
        LineItem ref{C(100), C(50), CreditDebit::DEBIT};
        LineItem same{C(100), C(50), CreditDebit::DEBIT};
        LineItem different{C(100), C(50), CreditDebit::CREDIT};

        REQUIRE(ref == ref);
        REQUIRE(ref == same);
        REQUIRE(ref != different);
    }
}

TEST_CASE("algorithms", "[LineItem]") {
    LineItem first{C(100), C(50), CreditDebit::CREDIT};
    LineItem second{C(100), C(70), CreditDebit::DEBIT};
    LineItem third{C(100), C(30), CreditDebit::CREDIT};

    SECTION("sum") {
        std::deque<LineItem> items{first, second, third};
        LineItem res = sum(items.begin(), items.end(), LineItem{});
        REQUIRE(res.taxable() == C(300));
        REQUIRE(res.payable() == C(10));
        REQUIRE(res.credit_debit() == CreditDebit::CREDIT);
    }
}