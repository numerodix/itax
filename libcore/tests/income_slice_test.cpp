#include "algos.h"
#include "income_slice.h"
#include "shortcuts.h"

#include "catch.hpp"
#include <list>

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

TEST_CASE("default constructor", "[IncomeSlice]") {
    IncomeSlice slice{};

    SECTION("accessors") {
        REQUIRE(slice.base() == C(0));
        REQUIRE(slice.amount() == C(0));
    }

    SECTION("bounds") {
        REQUIRE(slice.lower_bound() == C(0));
        REQUIRE(slice.upper_bound() == C(0));
    }
}

TEST_CASE("special member functions", "[IncomeSlice]") {
    SECTION("copy constructor") {
        IncomeSlice slice{C(10), C(20)};
        IncomeSlice other{slice};
        REQUIRE(other.base() == C(10));
        REQUIRE(other.amount() == C(20));
    }

    SECTION("copy assignment") {
        IncomeSlice slice{C(10), C(20)};
        IncomeSlice other = slice;
        REQUIRE(other.base() == C(10));
        REQUIRE(other.amount() == C(20));
    }

    SECTION("move constructor") {
        IncomeSlice slice{C(10), C(20)};
        IncomeSlice other{std::move(slice)};
        REQUIRE(other.base() == C(10));
        REQUIRE(other.amount() == C(20));
    }

    SECTION("move assignment") {
        IncomeSlice slice{C(10), C(20)};
        IncomeSlice other = std::move(slice);
        REQUIRE(other.base() == C(10));
        REQUIRE(other.amount() == C(20));
    }
}

TEST_CASE("addition", "[IncomeSlice]") {
    IncomeSlice first{C(0), C(20000)};
    IncomeSlice second{C(20000), C(30000)};
    IncomeSlice third{C(50000), C(10000)};

    SECTION("addition succeeds for adjacent, lower is first") {
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

    SECTION("addition succeeds for adjacent, upper is first") {
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

    SECTION("addition throws for non-adjacent slices, lower is first") {
        REQUIRE_THROWS_MATCHES(first + third, std::invalid_argument,
                               Message("cannot add non-adjacent slices"));
    }

    SECTION("addition throws for non-adjacent slices, upper is first") {
        REQUIRE_THROWS_MATCHES(third + first, std::invalid_argument,
                               Message("cannot add non-adjacent slices"));
    }
}

TEST_CASE("relational operators", "[IncomeSlice]") {
    SECTION("equality operator") {
        IncomeSlice ref{C(10), C(20)};
        IncomeSlice same{C(10), C(20)};
        IncomeSlice different{C(0), C(20)};

        REQUIRE(ref == ref);
        REQUIRE(ref == same);
        REQUIRE(ref != different);
    }
}

TEST_CASE("algorithms", "[IncomeSlice]") {
    IncomeSlice first{C(0), C(20000)};
    IncomeSlice second{C(20000), C(30000)};
    IncomeSlice third{C(50000), C(10000)};

    SECTION("sum") {
        std::list<IncomeSlice> slices{first, second, third};
        auto res = sum(slices.begin(), slices.end(), IncomeSlice{});
        REQUIRE(res.base() == C(0));
        REQUIRE(res.amount() == C(60000));
    }
}