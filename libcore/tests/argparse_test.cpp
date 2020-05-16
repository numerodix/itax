#include "argparse.h"
#include "shortcuts.h"

#include "catch.hpp"

using Catch::Matchers::Message;
using core::ArgParser;
using core::CashAmount;

TEST_CASE("tokenize") {
    ArgParser parser{};

    SECTION("program only") {
        char *argv[] = {"program"};
        const int argc = 1;

        auto tokens = parser.tokenize(argc, argv);

        REQUIRE(tokens.size() == 1);
        REQUIRE(tokens[0] == "program");
    }

    SECTION("program and a few args") {
        char *argv[] = {"program", "one", "two", "|", "three"};
        const int argc = 5;

        auto tokens = parser.tokenize(argc, argv);

        REQUIRE(tokens.size() == 5);
        REQUIRE(tokens[0] == "program");
        REQUIRE(tokens[1] == "one");
        REQUIRE(tokens[2] == "two");
        REQUIRE(tokens[3] == "|");
        REQUIRE(tokens[4] == "three");
    }
}

TEST_CASE("parse_returns") {
    ArgParser parser{};

    SECTION("program only") {
        std::vector<std::string> args{"program"};

        auto returns = parser.parse_returns(args);

        REQUIRE(returns.size() == 0);
    }

    SECTION("invalid number input #1") {
        std::vector<std::string> args{"program", "a150"};

        REQUIRE_THROWS_MATCHES(
            parser.parse_returns(args), std::invalid_argument,
            Message("invalid argument passed to parse_number()"));
    }

    SECTION("invalid number input #2") {
        std::vector<std::string> args{"program", "150 "};

        REQUIRE_THROWS_MATCHES(
            parser.parse_returns(args), std::invalid_argument,
            Message("invalid argument passed to parse_number()"));
    }

    SECTION("invalid number input #3") {
        std::vector<std::string> args{"program", "15mb"};

        REQUIRE_THROWS_MATCHES(
            parser.parse_returns(args), std::invalid_argument,
            Message("invalid argument passed to parse_number()"));
    }

    SECTION("invalid number input #4") {
        std::vector<std::string> args{"program", "15g"};

        REQUIRE_THROWS_MATCHES(
            parser.parse_returns(args), std::invalid_argument,
            Message("invalid argument passed to parse_number()"));
    }

    SECTION("single slice") {
        std::vector<std::string> args{"program", "150"};

        auto returns = parser.parse_returns(args);

        REQUIRE(returns.size() == 1);
        REQUIRE(returns[0].slices().size() == 1);
        REQUIRE(returns[0].total_income() == C(150));
    }

    SECTION("single three slices") {
        std::vector<std::string> args{"program", "19k", "12.5k", "3.12k",
                                      "5415"};

        auto returns = parser.parse_returns(args);

        REQUIRE(returns.size() == 1);
        REQUIRE(returns[0].slices().size() == 4);
        REQUIRE(returns[0].total_income() == C(40035));

        REQUIRE(returns[0].slices()[0].amount() == C(19000));
        REQUIRE(returns[0].slices()[1].amount() == C(12500));
        REQUIRE(returns[0].slices()[2].amount() == C(3120));
        REQUIRE(returns[0].slices()[3].amount() == C(5415));
    }
}

TEST_CASE("parse") {
    ArgParser parser{};

    SECTION("program and a few args") {
        char *argv[] = {"program", "11k", "22.3k"};
        const int argc = 3;

        auto returns = parser.parse(argc, argv);

        REQUIRE(returns.size() == 1);
        REQUIRE(returns[0].slices().size() == 2);
        REQUIRE(returns[0].total_income() == C(33300));
    }
}