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