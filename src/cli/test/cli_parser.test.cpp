#include "cli_parser.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("cli_parser parse_cmdline")
{
    cli_parser parser;

    SECTION("no operation")
    {
        const char* argv[] = {"hsmctl"};

        auto parsed_command = parser.parse_cmdline(1, argv);

        REQUIRE(parsed_command.operation == Operation::NONE);
        REQUIRE(parsed_command.error == ParseError::MISSING_OPERATION);
    }

    SECTION("invalid operation")
    {
        const char* argv[] = {"hsmctl", "invalid-operation"};

        auto result = parser.parse_cmdline(2, argv);

        REQUIRE(result.operation == Operation::NONE);
        REQUIRE(result.error == ParseError::INVALID_OPERATION);
    }

    SECTION("--help")
    {
        SECTION("success")
        {
            const char* argv[] = {"hsmctl", "--help"};

            auto parsed_command = parser.parse_cmdline(2, argv);

            REQUIRE(parsed_command.operation == Operation::HELP);
            REQUIRE(parsed_command.error == ParseError::NONE);
        }

        SECTION("invalid option")
        {
            const char* argv[] = {"hsmctl", "--help", "--some-option"};

            auto parsed_command = parser.parse_cmdline(3, argv);

            REQUIRE(parsed_command.operation == Operation::HELP);
            REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
        }
    }

    SECTION("status")
    {
        SECTION("success")
        {
            const char* argv[] = {"hsmctl", "status"};

            auto parsed_command = parser.parse_cmdline(2, argv);

            REQUIRE(parsed_command.operation == Operation::STATUS);
            REQUIRE(parsed_command.error == ParseError::NONE);
        }

        SECTION("help")
        {
            const char* argv[] = {"hsmctl", "status", "--help"};

            auto parsed_command = parser.parse_cmdline(3, argv);

            REQUIRE(parsed_command.operation == Operation::STATUS);
            REQUIRE(parsed_command.error == ParseError::NONE);
            REQUIRE(parsed_command.help == true);
        }

        SECTION("invalid option")
        {
            const char* argv[] = {"hsmctl", "status", "--some-option"};

            auto parsed_command = parser.parse_cmdline(3, argv);

            REQUIRE(parsed_command.operation == Operation::STATUS);
            REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
        }
    }

    SECTION("erase-key")
    {
        SECTION("success")
        {
            const char* argv[] = {"hsmctl", "erase-key", "--slot", "2"};

            auto parsed_command = parser.parse_cmdline(4, argv);

            REQUIRE(parsed_command.operation == Operation::ERASE_KEY);
            REQUIRE(parsed_command.options["slot"] == "2");
            REQUIRE(parsed_command.error == ParseError::NONE);
        }

        SECTION("help")
        {
            const char* argv[] = {"hsmctl", "erase-key", "--help"};

            auto parsed_command = parser.parse_cmdline(3, argv);

            REQUIRE(parsed_command.operation == Operation::ERASE_KEY);
            REQUIRE(parsed_command.error == ParseError::NONE);
            REQUIRE(parsed_command.help == true);
        }

        SECTION("missing option")
        {
            const char* argv[] = {"hsmctl", "erase-key"};

            auto parsed_command = parser.parse_cmdline(2, argv);

            REQUIRE(parsed_command.operation == Operation::ERASE_KEY);
            REQUIRE(parsed_command.error == ParseError::MISSING_OPTION);
        }

        SECTION("invalid option")
        {
            SECTION("unknown option")
            {
                const char* argv[] = {"hsmctl", "erase-key", "--invalid-option"};

                auto parsed_command = parser.parse_cmdline(3, argv);

                REQUIRE(parsed_command.operation == Operation::ERASE_KEY);
                REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
            }
            SECTION("trailing garbage after valid command")
            {
                const char* argv[] = {"hsmctl", "erase-key", "--slot", "0", "garbage"};

                auto parsed_command = parser.parse_cmdline(5, argv);

                REQUIRE(parsed_command.operation == Operation::ERASE_KEY);
                REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
            }
        }

        SECTION("missing slot value")
        {
            const char* argv[] = {"hsmctl", "erase-key", "--slot"};

            auto parsed_command = parser.parse_cmdline(3, argv);

            REQUIRE(parsed_command.operation == Operation::ERASE_KEY);
            REQUIRE(parsed_command.error == ParseError::MISSING_VALUE);
        }

        SECTION("invalid slot value")
        {
            SECTION("not a number")
            {
                const char* argv[] = {"hsmctl", "erase-key", "--slot", "NaN"};

                auto result = parser.parse_cmdline(4, argv);

                REQUIRE(result.operation == Operation::ERASE_KEY);
                REQUIRE(result.error == ParseError::INVALID_VALUE);
            }

            SECTION("out of boundaries")
            {
                const char* argv[] = {"hsmctl", "erase-key", "--slot", "32"};

                auto result = parser.parse_cmdline(4, argv);

                REQUIRE(result.operation == Operation::ERASE_KEY);
                REQUIRE(result.error == ParseError::INVALID_VALUE);
            }
        }
    }
}
