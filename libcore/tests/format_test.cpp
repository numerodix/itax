#include "cash_amount.h"
#include "format.h"
#include "shortcuts.h"

#include "catch.hpp"

using core::CashAmount;

TEST_CASE("format_plain", "[format]") {
    SECTION("format_plain #1") {
        CashAmount amount{12345678950L};
        REQUIRE(format_plain(amount) == "1234567.90");
    }
}

TEST_CASE("format_with_commas", "[format]") {
    SECTION("format_with_commas() formats with commas #1") {
        CashAmount amount{12345678950L};
        REQUIRE(format_with_commas(amount) == "1,234,567.90");
    }

    SECTION("format_with_commas() formats with commas #2") {
        CashAmount amount{2345678950L};
        REQUIRE(format_with_commas(amount) == "234,567.90");
    }

    SECTION("format_with_commas() formats with commas #3") {
        CashAmount amount{5678950L};
        REQUIRE(format_with_commas(amount) == "567.90");
    }

    SECTION("format_with_commas() formats with commas #4") {
        CashAmount amount{8950L};
        REQUIRE(format_with_commas(amount) == "0.90");
    }

    SECTION("format_with_commas() formats with commas #5") {
        CashAmount amount{120000L};
        REQUIRE(format_with_commas(amount) == "12.00");
    }
}