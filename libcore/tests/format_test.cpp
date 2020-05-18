#include "cash_amount.h"
#include "format.h"
#include "shortcuts.h"

#include "catch.hpp"

using core::CashAmount;
using core::CreditDebit;
using core::format_percent;
using core::format_plain;
using core::format_with_commas;

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

TEST_CASE("format_percent", "[format]") {
    SECTION("format_percent #1") { REQUIRE(format_percent(0.0) == "00.00"); }

    SECTION("format_percent #2") { REQUIRE(format_percent(0.01) == "01.00"); }

    SECTION("format_percent #3") { REQUIRE(format_percent(0.21) == "21.00"); }

    SECTION("format_percent #4") { REQUIRE(format_percent(0.214) == "21.40"); }

    SECTION("format_percent #5") { REQUIRE(format_percent(0.2147) == "21.47"); }

    SECTION("format_percent #6") {
        REQUIRE(format_percent(0.214792) == "21.48");
    }

    SECTION("format_percent #7") {
        REQUIRE(format_percent(2.2147) == "221.47");
    }
}

TEST_CASE("format_sign", "[format]") {
    SECTION("format_sign #1") {
        REQUIRE(format_sign(CreditDebit::CREDIT) == " ");
    }

    SECTION("format_sign #2") {
        REQUIRE(format_sign(CreditDebit::DEBIT) == "-");
    }
}