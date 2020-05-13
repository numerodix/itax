#include "income_slice.h"
#include "shortcuts.h"

#include "catch.hpp"

using Catch::Matchers::Message;
using core::CashAmount;
using core::IncomeSlice;

TEST_CASE("basic properties", "[IncomeSlice]") {
    IncomeSlice slice{C(20000), C(15525.50)};

    SECTION("accessors") {
        REQUIRE(slice.base() == C(20000));
        REQUIRE(slice.amount() == C(15525.50));
    }

    SECTION("bounds") {
        REQUIRE(slice.lower_bound() == C(20000));
        REQUIRE(slice.upper_bound() == C(35525.50));
    }
}

TEST_CASE("merging", "[IncomeSlice]") {
    IncomeSlice first{C(0), C(20000)};
    IncomeSlice second{C(20000), C(30000)};
    IncomeSlice third{C(50000), C(10000)};

    SECTION("merge succeeds for adjacent, lower is first") {
        IncomeSlice first_second = first + second;
        REQUIRE(first_second.lower_bound() == C(0));
        REQUIRE(first_second.upper_bound() == C(50000));

        IncomeSlice second_third = second + third;
        REQUIRE(second_third.lower_bound() == C(20000));
        REQUIRE(second_third.upper_bound() == C(60000));

        IncomeSlice all_three = first + second + third;
        REQUIRE(all_three.lower_bound() == C(0));
        REQUIRE(all_three.upper_bound() == C(60000));
    }

    SECTION("merge succeeds for adjacent, upper is first") {
        IncomeSlice first_second = second + first;
        REQUIRE(first_second.lower_bound() == C(0));
        REQUIRE(first_second.upper_bound() == C(50000));

        IncomeSlice second_third = third + second;
        REQUIRE(second_third.lower_bound() == C(20000));
        REQUIRE(second_third.upper_bound() == C(60000));

        IncomeSlice all_three = third + second + first;
        REQUIRE(all_three.lower_bound() == C(0));
        REQUIRE(all_three.upper_bound() == C(60000));
    }

    SECTION("merge throws for non-adjacent slices, lower is first") {
        REQUIRE_THROWS_MATCHES(
            first + third, std::invalid_argument,
            Message("cannot add non-adjacent slices"));
    }

    SECTION("merge throws for non-adjacent slices, upper is first") {
        REQUIRE_THROWS_MATCHES(
            third + first, std::invalid_argument,
            Message("cannot add non-adjacent slices"));
    }
}