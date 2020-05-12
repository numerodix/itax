#include "catch.hpp"

#include "currency_amount.h"

using Catch::Matchers::Message;
using core::CurrencyAmount;

TEST_CASE("basic properties", "[CurrencyAmount]") {
    SECTION("aborts if constructed with a negative value") {
        REQUIRE_THROWS_MATCHES(
            CurrencyAmount{-1L}, std::out_of_range,
            Message("constructor called with a negative number"));
    }

    SECTION("raw() returns the underlying int") {
        CurrencyAmount amount{12345678900L};
        REQUIRE(amount.raw() == 12345678900L);
    }

    SECTION("value_part() returns the value part") {
        CurrencyAmount amount{12345678950L};
        REQUIRE(amount.value_part() == 123456789L);
    }

    SECTION("rounding_part() returns the rounding part") {
        CurrencyAmount amount{12345678950L};
        REQUIRE(amount.rounding_part() == 50L);
    }

    SECTION("rounded_value_part() returns the value part, rounded") {
        CurrencyAmount round_down{12345678940L};
        REQUIRE(round_down.rounded_value_part() == 123456789L);

        CurrencyAmount round_up{12345678950L};
        REQUIRE(round_up.rounded_value_part() == 123456790L);
    }
}

TEST_CASE("non-trivial construction", "[CurrencyAmount]") {
    SECTION("invalid string input") {
        REQUIRE_THROWS_MATCHES(CurrencyAmount::from("forty seven"),
                               std::invalid_argument,
                               Message("from() called with an input that could "
                                       "not be parsed as an amount"));
    }

    SECTION("constructed from string") {
        CurrencyAmount amount = CurrencyAmount::from("123.45");
        REQUIRE(amount.raw() == 1234500L);
    }

    SECTION("constructed from string when value is integer zero") {
        CurrencyAmount amount = CurrencyAmount::from("0");
        REQUIRE(amount.raw() == 0L);
    }

    SECTION("constructed from string when value is floating point zero") {
        CurrencyAmount amount = CurrencyAmount::from("0.0");
        REQUIRE(amount.raw() == 0L);
    }
}

TEST_CASE("display routines", "[CurrencyAmount]") {
    SECTION("display_plain() returns a string representation, rounded") {
        CurrencyAmount amount{12345678950L};
        REQUIRE(amount.display_plain() == "1234567.90");
    }

    SECTION("display_with_commas() formats with commas #1") {
        CurrencyAmount amount{12345678950L};
        REQUIRE(amount.display_with_commas() == "1,234,567.90");
    }

    SECTION("display_with_commas() formats with commas #2") {
        CurrencyAmount amount{2345678950L};
        REQUIRE(amount.display_with_commas() == "234,567.90");
    }

    SECTION("display_with_commas() formats with commas #3") {
        CurrencyAmount amount{5678950L};
        REQUIRE(amount.display_with_commas() == "567.90");
    }
}

TEST_CASE("relational operators", "[CurrencyAmount]") {
    SECTION("equality operator") {
        CurrencyAmount ref{1295L};
        CurrencyAmount same{1295L};
        CurrencyAmount different{1296L};

        REQUIRE(ref == ref);
        REQUIRE(ref == same);
        REQUIRE(ref != different);
    }

    SECTION("lesser/greater operator") {
        CurrencyAmount ref{1295L};
        CurrencyAmount same{1295L};
        CurrencyAmount smaller{1294L};
        CurrencyAmount greater{1296L};

        REQUIRE(ref > smaller);
        REQUIRE(ref < greater);

        REQUIRE(ref <= same);
        REQUIRE(ref >= same);

        REQUIRE(ref >= smaller);
        REQUIRE(ref <= greater);
    }
}

TEST_CASE("arithmetic routines", "[CurrencyAmount]") {
    SECTION("addition operator") {
        CurrencyAmount left{1295L};
        CurrencyAmount right{705L};
        CurrencyAmount res = left + right;
        REQUIRE(res.raw() == 2000L);
    }

    SECTION("subtraction operator") {
        CurrencyAmount left{1295L};
        CurrencyAmount right{705L};
        CurrencyAmount res = left - right;
        REQUIRE(res.raw() == 590L);
    }

    SECTION("multiplication operator left operand") {
        CurrencyAmount left{1100L};
        double right = 0.5;
        CurrencyAmount res = left * right;
        REQUIRE(res.raw() == 550L);
    }

    SECTION("multiplication operator right operand") {
        double left = 0.5;
        CurrencyAmount right{1100L};
        CurrencyAmount res = left * right;
        REQUIRE(res.raw() == 550L);
    }

    SECTION("division operator") {
        CurrencyAmount left{1100L};
        double right = 3.0;
        CurrencyAmount res = left / right;
        REQUIRE(res.raw() == 366L);
    }
}