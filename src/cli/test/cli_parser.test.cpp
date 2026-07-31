#include "cli_parser.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("parse_cmdline")
{
    cli_parser parser;

    SECTION("returns UNKNOWN for unrecognized command")
    {
        const char* argv[] = {"hsmctl"};
        auto result = parser.parse_cmdline(1, argv);
        REQUIRE(result == Command::UNKNOWN);
    }

    SECTION("returns HELP command")
    {
        const char* argv[] = {"hsmctl", "--help"};
        auto result = parser.parse_cmdline(2, argv);
        REQUIRE(result == Command::HELP);
    }

    SECTION("returns STATUS command")
    {
        const char* argv[] = {"hsmctl", "status"};
        auto result = parser.parse_cmdline(2, argv);
        REQUIRE(result == Command::STATUS);
    }
}
