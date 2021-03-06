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

    SECTION("slices") {
        REQUIRE(taxret.slices()[0] == first);
        REQUIRE(taxret.slices()[1] == second);
    }
    SECTION("total_income") { REQUIRE(taxret.total_income() == C(50000)); }
}

TEST_CASE("constructor given zero slices", "[TaxReturn]") {
    std::vector<IncomeSlice> slices{};
    TaxReturn taxret{slices};

    SECTION("slices") { REQUIRE(taxret.slices().size() == 0); }
    SECTION("total_income") { REQUIRE(taxret.total_income() == C(0)); }
}

TEST_CASE("constructor validates slices, base not 0", "[TaxReturn]") {
    IncomeSlice first{C(10000), C(10000)};
    IncomeSlice second{C(20000), C(30000)};
    std::vector<IncomeSlice> slices = {{first, second}};

    SECTION("throws when sum of slices has a base greater than 0") {
        REQUIRE_THROWS_MATCHES(TaxReturn{slices}, std::invalid_argument,
                               Message("sum of slices must have a base of 0"));
    }
}

TEST_CASE("constructor validates slices, slices not adjacent", "[TaxReturn]") {
    IncomeSlice first{C(0), C(10000)};
    IncomeSlice second{C(20000), C(30000)};
    std::vector<IncomeSlice> slices = {{first, second}};

    SECTION("throws when slices are not adjacent") {
        REQUIRE_THROWS_MATCHES(TaxReturn{slices}, std::invalid_argument,
                               Message("cannot add non-adjacent slices"));
    }
}

TEST_CASE("special member functions", "[TaxReturn]") {
    IncomeSlice first{C(0), C(20000)};
    IncomeSlice second{C(20000), C(30000)};
    std::vector<IncomeSlice> slices = {{first, second}};

    TaxReturn taxret{slices};

    SECTION("copy constructor") {
        TaxReturn other{taxret};
        REQUIRE(other.slices()[0] == first);
        REQUIRE(other.slices()[1] == second);
    }

    SECTION("copy assignment") {
        TaxReturn other = taxret;
        REQUIRE(other.slices()[0] == first);
        REQUIRE(other.slices()[1] == second);
    }

    SECTION("move constructor") {
        TaxReturn other{std::move(taxret)};
        REQUIRE(other.slices()[0] == first);
        REQUIRE(other.slices()[1] == second);
    }

    SECTION("move assignment") {
        TaxReturn other = std::move(taxret);
        REQUIRE(other.slices()[0] == first);
        REQUIRE(other.slices()[1] == second);
    }
}