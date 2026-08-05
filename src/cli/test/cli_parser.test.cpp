#include "cli_parser.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("cli_parser parse_cmdline")
{
    cli_parser parser;

    SECTION("success")
    {
        SECTION("--help")
        {
            const char* argv[] = {"hsmctl", "--help"};
            auto parsed_command = parser.parse_cmdline(2, argv);
            REQUIRE(parsed_command.operation == Operation::HELP);
        }

        SECTION("status")
        {
            const char* argv[] = {"hsmctl", "status"};
            auto parsed_command = parser.parse_cmdline(2, argv);
            REQUIRE(parsed_command.operation == Operation::STATUS);
        }

        SECTION("erase-key")
        {
            const char* argv[] = {"hsmctl", "erase-key", "--slot", "2"};

            auto parsed_command = parser.parse_cmdline(4, argv);

            REQUIRE(parsed_command.operation == Operation::ERASE_KEY);
            REQUIRE(parsed_command.options["slot"] == "2");
        }
    }

    SECTION("returns MISSING_OPERATION parse error")
    {
        const char* argv[] = {"hsmctl"};
        auto parsed_command = parser.parse_cmdline(1, argv);

        REQUIRE(parsed_command.operation == Operation::NONE);
        REQUIRE(parsed_command.error == ParseError::MISSING_OPERATION);
    }

    SECTION("returns MISSING_OPTION parse error")
    {
        const char* argv[] = {"hsmctl", "erase-key"};

        auto parsed_command = parser.parse_cmdline(2, argv);

        REQUIRE(parsed_command.operation == Operation::ERASE_KEY);
        REQUIRE(parsed_command.error == ParseError::MISSING_OPTION);
    }

    SECTION("returns INVALID_OPTION parse error")
    {
        const char* argv[] = {"hsmctl", "erase-key", "--invalid-option"};

        auto parsed_command = parser.parse_cmdline(3, argv);

        REQUIRE(parsed_command.operation == Operation::ERASE_KEY);
        REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
    }

    SECTION("returns MISSING_SPECIFIER parse error")
    {
        const char* argv[] = {"hsmctl", "erase-key", "--slot"};

        auto parsed_command = parser.parse_cmdline(3, argv);

        REQUIRE(parsed_command.operation == Operation::ERASE_KEY);
        REQUIRE(parsed_command.error == ParseError::MISSING_VALUE);
    }

    SECTION("returns INVALID_SPECIFIER parse error")
    {
        const char* argv[] = {"hsmctl", "erase-key", "--slot"};

        SECTION("key slot out of range")
        {
            argv[3] = "32";

            auto parsed_command = parser.parse_cmdline(4, argv);

            REQUIRE(parsed_command.operation == Operation::ERASE_KEY);
            REQUIRE(parsed_command.error == ParseError::INVALID_VALUE);
        }

        SECTION("key slot not a number")
        {
            argv[3] = "NaN";

            auto parsed_command = parser.parse_cmdline(4, argv);

            REQUIRE(parsed_command.operation == Operation::ERASE_KEY);
            REQUIRE(parsed_command.error == ParseError::INVALID_VALUE);
        }
    }
}
