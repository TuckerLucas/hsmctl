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

    SECTION("returns INJECT_KEY command")
    {
        const char* argv[] = {"hsmctl", "inject-key"};
        auto result = parser.parse_cmdline(2, argv);
        REQUIRE(result == Command::INJECT_KEY);
    }

    SECTION("returns UPDATE_KEY command")
    {
        const char* argv[] = {"hsmctl", "update-key"};
        auto result = parser.parse_cmdline(2, argv);
        REQUIRE(result == Command::UPDATE_KEY);
    }

    SECTION("returns GET_KEY command")
    {
        const char* argv[] = {"hsmctl", "get-key"};
        auto result = parser.parse_cmdline(2, argv);
        REQUIRE(result == Command::GET_KEY);
    }

    SECTION("returns ERASE_KEY command")
    {
        const char* argv[] = {"hsmctl", "erase-key"};
        auto result = parser.parse_cmdline(2, argv);
        REQUIRE(result == Command::ERASE_KEY);
    }
}
