#include "catch.hpp"

#include "currency_amount.h"

TEST_CASE("first test", "[amt]") {
    core::CurrencyAmount amt{1234678900L};

    REQUIRE(amt.m_amount == 1234678900L);
}