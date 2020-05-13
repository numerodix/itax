#include "shortcuts.h"
#include "tax_return.h"

#include "catch.hpp"

using Catch::Matchers::Message;
using core::CashAmount;
using core::IncomeSlice;
using core::TaxReturn;

TEST_CASE("basic properties", "[TaxReturn]") {
    IncomeSlice first{C(0), C(20000)};
    IncomeSlice second{C(20000), C(30000)};
    std::vector<IncomeSlice> slices = {{first, second}};

    TaxReturn taxret{slices};

    SECTION("total_income") { REQUIRE(taxret.total_income() == C(50000)); }
}

TEST_CASE("constructor validates slices", "[TaxReturn]") {
    IncomeSlice first{C(10000), C(10000)};
    IncomeSlice second{C(20000), C(30000)};
    std::vector<IncomeSlice> slices = {{first, second}};

    SECTION("throws when sum of slices has a base greater than 0") {
        REQUIRE_THROWS_MATCHES(TaxReturn{slices}, std::invalid_argument,
                               Message("sum of slices must have a base of 0"));
    }
}
