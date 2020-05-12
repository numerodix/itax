#include "catch.hpp"

#include "currency_amount.h"

TEST_CASE("basic properties", "[CurrencyAmount]") {
    core::CurrencyAmount amount{1234678900L};

    SECTION("raw() returns the underlying int") {
        REQUIRE(amount.raw() == 1234678900L);
    }
}