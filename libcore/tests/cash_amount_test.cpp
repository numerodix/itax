#include "catch.hpp"

#include "cash_amount.h"

using Catch::Matchers::Message;
using core::CashAmount;

TEST_CASE("basic properties", "[CashAmount]") {
    SECTION("aborts if constructed with a negative value") {
        REQUIRE_THROWS_MATCHES(
            CashAmount{-1L}, std::out_of_range,
            Message("constructor called with a negative number"));
    }

    SECTION("raw() returns the underlying int") {
        CashAmount amount{12345678900L};
        REQUIRE(amount.raw() == 12345678900L);
    }

    SECTION("value_part() returns the value part") {
        CashAmount amount{12345678950L};
        REQUIRE(amount.value_part() == 123456789L);
    }

    SECTION("rounding_part() returns the rounding part") {
        CashAmount amount{12345678950L};
        REQUIRE(amount.rounding_part() == 50L);
    }

    SECTION("rounded_value_part() returns the value part, rounded") {
        CashAmount round_down{12345678940L};
        REQUIRE(round_down.rounded_value_part() == 123456789L);

        CashAmount round_up{12345678950L};
        REQUIRE(round_up.rounded_value_part() == 123456790L);
    }
}

TEST_CASE("non-trivial construction", "[CashAmount]") {
    SECTION("invalid string input") {
        REQUIRE_THROWS_MATCHES(CashAmount::from("forty seven"),
                               std::invalid_argument,
                               Message("from() called with an input that could "
                                       "not be parsed as an amount"));
    }

    SECTION("constructed from string #1") {
        CashAmount amount = CashAmount::from("123.45");
        REQUIRE(amount.raw() == 1234500L);
    }

    SECTION("constructed from string #2") {
        CashAmount amount = CashAmount::from("123");
        REQUIRE(amount.raw() == 1230000L);
    }

    SECTION("constructed from string when value is integer zero") {
        CashAmount amount = CashAmount::from("0");
        REQUIRE(amount.raw() == 0L);
    }

    SECTION("constructed from string when value is floating point zero") {
        CashAmount amount = CashAmount::from("0.0");
        REQUIRE(amount.raw() == 0L);
    }
}

TEST_CASE("display routines", "[CashAmount]") {
    SECTION("display_plain() returns a string representation, rounded") {
        CashAmount amount{12345678950L};
        REQUIRE(amount.display_plain() == "1234567.90");
    }

    SECTION("display_with_commas() formats with commas #1") {
        CashAmount amount{12345678950L};
        REQUIRE(amount.display_with_commas() == "1,234,567.90");
    }

    SECTION("display_with_commas() formats with commas #2") {
        CashAmount amount{2345678950L};
        REQUIRE(amount.display_with_commas() == "234,567.90");
    }

    SECTION("display_with_commas() formats with commas #3") {
        CashAmount amount{5678950L};
        REQUIRE(amount.display_with_commas() == "567.90");
    }

    SECTION("display_with_commas() formats with commas #4") {
        CashAmount amount{8950L};
        REQUIRE(amount.display_with_commas() == "0.90");
    }
}

TEST_CASE("relational operators", "[CashAmount]") {
    SECTION("equality operator") {
        CashAmount ref{1295L};
        CashAmount same{1295L};
        CashAmount different{1296L};

        REQUIRE(ref == ref);
        REQUIRE(ref == same);
        REQUIRE(ref != different);
    }

    SECTION("lesser/greater operator") {
        CashAmount ref{1295L};
        CashAmount same{1295L};
        CashAmount smaller{1294L};
        CashAmount greater{1296L};

        REQUIRE(ref > smaller);
        REQUIRE(ref < greater);

        REQUIRE(ref <= same);
        REQUIRE(ref >= same);

        REQUIRE(ref >= smaller);
        REQUIRE(ref <= greater);
    }
}

TEST_CASE("arithmetic routines", "[CashAmount]") {
    SECTION("addition operator") {
        CashAmount left{1295L};
        CashAmount right{705L};
        CashAmount res = left + right;
        REQUIRE(res.raw() == 2000L);
    }

    SECTION("subtraction operator") {
        CashAmount left{1295L};
        CashAmount right{705L};
        CashAmount res = left - right;
        REQUIRE(res.raw() == 590L);
    }

    SECTION("multiplication operator lhs operand") {
        CashAmount left{1100L};
        double right = 0.5;
        CashAmount res = left * right;
        REQUIRE(res.raw() == 550L);
    }

    SECTION("multiplication operator rhs operand") {
        double left = 0.5;
        CashAmount right{1100L};
        CashAmount res = left * right;
        REQUIRE(res.raw() == 550L);
    }

    SECTION("division operator") {
        CashAmount left{1100L};
        double right = 3.0;
        CashAmount res = left / right;
        REQUIRE(res.raw() == 366L);
    }
}