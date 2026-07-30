#include "cli_parser.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("parse_cmdline")
{
    cli_parser parser;

    SECTION("No arguments")
    {
        const char* argv[] = {"hsmctl"};
        auto result = parser.parse_cmdline(1, argv);
        REQUIRE(result == Command::UNKNOWN);
    }

    SECTION("--help")
    {
        const char* argv[] = {"hsmctl", "--help"};
        auto result = parser.parse_cmdline(2, argv);
        REQUIRE(result == Command::HELP);
    }
}
