#include "bracket.h"
#include "shortcuts.h"

#include "catch.hpp"

using Catch::Matchers::Message;
using core::Bracket;
using core::CashAmount;
using core::IncomeSlice;

TEST_CASE("basic properties", "[Bracket]") {
    Bracket bracket{C(100), C(250)};

    SECTION("accessors") {
        REQUIRE(bracket.lower() == C(100));
        REQUIRE(bracket.upper() == C(250));
        REQUIRE(bracket.range() == C(150));
    }

    SECTION("aborts if constructed with a negative range") {
        REQUIRE_THROWS_MATCHES(
            (Bracket{C(2), C(1)}), std::invalid_argument,
            Message("constructor called with a negative range"));
    }
}

TEST_CASE("special member functions", "[Bracket]") {
    SECTION("copy constructor") {
        Bracket bracket{C(100), C(200)};
        Bracket other{bracket};
        REQUIRE(other.lower() == C(100));
        REQUIRE(other.upper() == C(200));
    }

    SECTION("copy assignment") {
        Bracket bracket{C(100), C(200)};
        Bracket other = bracket;
        REQUIRE(other.lower() == C(100));
        REQUIRE(other.upper() == C(200));
    }

    SECTION("move constructor") {
        Bracket bracket{C(100), C(200)};
        Bracket other{std::move(bracket)};
        REQUIRE(other.lower() == C(100));
        REQUIRE(other.upper() == C(200));
    }

    SECTION("move assignment") {
        Bracket bracket{C(100), C(200)};
        Bracket other = std::move(bracket);
        REQUIRE(other.lower() == C(100));
        REQUIRE(other.upper() == C(200));
    }
}

TEST_CASE("in_bracket", "[Bracket]") {
    SECTION("below the bracket, with a margin") {
        Bracket bracket{C(17000), C(31000)};
        IncomeSlice slice{C(0), C(15000)};

        auto res = bracket.in_bracket(slice);

        REQUIRE(res == C(0));
    }

    SECTION("below the bracket, adjacent") {
        Bracket bracket{C(17000), C(31000)};
        IncomeSlice slice{C(0), C(17000)};

        auto res = bracket.in_bracket(slice);

        REQUIRE(res == C(0));
    }

    SECTION("starts below the bracket, ends inside the bracket") {
        Bracket bracket{C(17000), C(31000)};
        IncomeSlice slice{C(16000), C(14000)};

        auto res = bracket.in_bracket(slice);

        REQUIRE(res == C(13000));
    }

    SECTION("starts below the bracket, ends at the end of the bracket") {
        Bracket bracket{C(17000), C(31000)};
        IncomeSlice slice{C(16000), C(15000)};

        auto res = bracket.in_bracket(slice);

        REQUIRE(res == C(14000));
    }

    SECTION("starts below the bracket, ends above the bracket") {
        Bracket bracket{C(17000), C(31000)};
        IncomeSlice slice{C(16000), C(16000)};

        auto res = bracket.in_bracket(slice);

        REQUIRE(res == C(14000));
    }

    SECTION("starts at the bracket, ends inside the bracket") {
        Bracket bracket{C(17000), C(31000)};
        IncomeSlice slice{C(17000), C(13000)};

        auto res = bracket.in_bracket(slice);

        REQUIRE(res == C(13000));
    }

    SECTION("starts at the bracket, ends at the end of the bracket") {
        Bracket bracket{C(17000), C(31000)};
        IncomeSlice slice{C(17000), C(14000)};

        auto res = bracket.in_bracket(slice);

        REQUIRE(res == C(14000));
    }

    SECTION("starts at the bracket, ends above the bracket") {
        Bracket bracket{C(17000), C(31000)};
        IncomeSlice slice{C(17000), C(15000)};

        auto res = bracket.in_bracket(slice);

        REQUIRE(res == C(14000));
    }

    SECTION("starts inside the bracket, ends inside the bracket") {
        Bracket bracket{C(17000), C(31000)};
        IncomeSlice slice{C(18000), C(12000)};

        auto res = bracket.in_bracket(slice);

        REQUIRE(res == C(12000));
    }

    SECTION("starts inside the bracket, ends at the end of the bracket") {
        Bracket bracket{C(17000), C(31000)};
        IncomeSlice slice{C(18000), C(13000)};

        auto res = bracket.in_bracket(slice);

        REQUIRE(res == C(13000));
    }

    SECTION("starts inside the bracket, ends above the bracket") {
        Bracket bracket{C(17000), C(31000)};
        IncomeSlice slice{C(18000), C(14000)};

        auto res = bracket.in_bracket(slice);

        REQUIRE(res == C(13000));
    }

    SECTION("above the bracket, with a margin") {
        Bracket bracket{C(17000), C(31000)};
        IncomeSlice slice{C(33000), C(11000)};

        auto res = bracket.in_bracket(slice);

        REQUIRE(res == C(0));
    }

    SECTION("above the bracket, adjacent") {
        Bracket bracket{C(17000), C(31000)};
        IncomeSlice slice{C(31000), C(11000)};

        auto res = bracket.in_bracket(slice);

        REQUIRE(res == C(0));
    }
}