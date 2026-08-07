#include "cli_parser.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string>

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

    SECTION("help")
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
            SECTION("non-boundary slot")
            {
                const char* argv[] = {"hsmctl", "erase-key", "--slot", "2"};

                auto parsed_command = parser.parse_cmdline(4, argv);

                REQUIRE(parsed_command.operation == Operation::ERASE_KEY);
                REQUIRE(parsed_command.options["slot"] == "2");
                REQUIRE(parsed_command.error == ParseError::NONE);
            }

            SECTION("boundary slots")
            {
                SECTION("lower boundary")
                {
                    std::string min_slot_str = std::to_string(MIN_SLOT);
                    const char* argv[] = {"hsmctl", "erase-key", "--slot", min_slot_str.c_str()};

                    auto parsed_command = parser.parse_cmdline(4, argv);

                    REQUIRE(parsed_command.operation == Operation::ERASE_KEY);
                    REQUIRE(parsed_command.options["slot"] == min_slot_str.c_str());
                    REQUIRE(parsed_command.error == ParseError::NONE);
                }

                SECTION("upper boundary")
                {
                    std::string max_slot_str = std::to_string(MAX_SLOT);
                    const char* argv[] = {"hsmctl", "erase-key", "--slot", max_slot_str.c_str()};

                    auto parsed_command = parser.parse_cmdline(4, argv);

                    REQUIRE(parsed_command.operation == Operation::ERASE_KEY);
                    REQUIRE(parsed_command.options["slot"] == max_slot_str.c_str());
                    REQUIRE(parsed_command.error == ParseError::NONE);
                }
            }
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

            SECTION("duplicate slot")
            {
                const char* argv[] = {"hsmctl", "erase-key", "--slot", "3", "--slot", "14"};

                auto parsed_command = parser.parse_cmdline(6, argv);

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

    SECTION("generate-key")
    {
        SECTION("success")
        {
            SECTION("unspecified curve")
            {
                const char* argv[] = {"hsmctl", "generate-key", "--slot", "10"};

                auto parsed_command = parser.parse_cmdline(4, argv);

                REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                REQUIRE(parsed_command.options["slot"] == "10");
                REQUIRE(parsed_command.options["curve"] == "ed25519");
                REQUIRE(parsed_command.error == ParseError::NONE);
            }

            SECTION("specified curve")
            {
                SECTION("ED25519")
                {
                    const char* argv[] = {"hsmctl", "generate-key", "--slot",
                                          "12",     "--curve",      "ed25519"};

                    auto parsed_command = parser.parse_cmdline(6, argv);

                    REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                    REQUIRE(parsed_command.options["slot"] == "12");
                    REQUIRE(parsed_command.options["curve"] == "ed25519");
                    REQUIRE(parsed_command.error == ParseError::NONE);
                }

                SECTION("NIST P-256")
                {
                    const char* argv[] = {"hsmctl", "generate-key", "--slot",
                                          "18",     "--curve",      "p256"};

                    auto parsed_command = parser.parse_cmdline(6, argv);

                    REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                    REQUIRE(parsed_command.options["slot"] == "18");
                    REQUIRE(parsed_command.options["curve"] == "p256");
                    REQUIRE(parsed_command.error == ParseError::NONE);
                }

                SECTION("reversed operation order")
                {
                    const char* argv[] = {"hsmctl", "generate-key", "--curve",
                                          "p256",   "--slot",       "16"};

                    auto parsed_command = parser.parse_cmdline(6, argv);

                    REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                    REQUIRE(parsed_command.options["slot"] == "16");
                    REQUIRE(parsed_command.options["curve"] == "p256");
                    REQUIRE(parsed_command.error == ParseError::NONE);
                }
            }

            SECTION("boundary slots")
            {
                SECTION("lower boundary")
                {
                    std::string min_slot_str = std::to_string(MIN_SLOT);
                    const char* argv[] = {"hsmctl", "generate-key", "--slot", min_slot_str.c_str()};

                    auto parsed_command = parser.parse_cmdline(4, argv);

                    REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                    REQUIRE(parsed_command.options["slot"] == min_slot_str.c_str());
                    REQUIRE(parsed_command.error == ParseError::NONE);
                }

                SECTION("upper boundary")
                {
                    std::string max_slot_str = std::to_string(MAX_SLOT);
                    const char* argv[] = {"hsmctl", "generate-key", "--slot", max_slot_str.c_str()};

                    auto parsed_command = parser.parse_cmdline(4, argv);

                    REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                    REQUIRE(parsed_command.options["slot"] == max_slot_str.c_str());
                    REQUIRE(parsed_command.error == ParseError::NONE);
                }
            }
        }

        SECTION("help")
        {
            const char* argv[] = {"hsmctl", "generate-key", "--help"};

            auto parsed_command = parser.parse_cmdline(3, argv);

            REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
            REQUIRE(parsed_command.error == ParseError::NONE);
            REQUIRE(parsed_command.help == true);
        }

        SECTION("missing option")
        {
            SECTION("unspecified curve")
            {
                const char* argv[] = {"hsmctl", "generate-key"};

                auto parsed_command = parser.parse_cmdline(2, argv);

                REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                REQUIRE(parsed_command.error == ParseError::MISSING_OPTION);
            }

            SECTION("specified curve")
            {
                const char* argv[] = {"hsmctl", "generate-key", "--curve", "p256"};

                auto parsed_command = parser.parse_cmdline(4, argv);

                REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                REQUIRE(parsed_command.error == ParseError::MISSING_OPTION);
            }
        }

        SECTION("invalid option")
        {
            SECTION("unknown option")
            {
                const char* argv[] = {"hsmctl", "generate-key", "--invalid-option"};

                auto parsed_command = parser.parse_cmdline(3, argv);

                REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
            }

            SECTION("trailing garbage after valid command")
            {
                const char* argv[] = {"hsmctl", "generate-key", "--slot", "0", "garbage"};

                auto parsed_command = parser.parse_cmdline(5, argv);

                REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
            }

            SECTION("duplicate options")
            {
                SECTION("slot")
                {
                    const char* argv[] = {"hsmctl", "generate-key", "--slot", "3", "--slot", "14"};

                    auto parsed_command = parser.parse_cmdline(6, argv);

                    REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                    REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
                }

                SECTION("curve")
                {
                    const char* argv[] = {"hsmctl",  "generate-key", "--slot",  "3",
                                          "--curve", "ed25519",      "--curve", "p256"};

                    auto parsed_command = parser.parse_cmdline(8, argv);

                    REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                    REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
                }
            }
        }

        SECTION("missing value")
        {
            SECTION("slot")
            {
                SECTION("unspecified curve")
                {
                    const char* argv[] = {"hsmctl", "generate-key", "--slot"};

                    auto parsed_command = parser.parse_cmdline(3, argv);

                    REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                    REQUIRE(parsed_command.error == ParseError::MISSING_VALUE);
                }

                SECTION("slot specified first")
                {
                    const char* argv[] = {"hsmctl", "generate-key", "--slot", "--curve", "p256"};

                    auto parsed_command = parser.parse_cmdline(5, argv);

                    REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                    REQUIRE(parsed_command.error == ParseError::MISSING_VALUE);
                }

                SECTION("curve specified first")
                {
                    const char* argv[] = {"hsmctl", "generate-key", "--curve", "p256", "--slot"};

                    auto parsed_command = parser.parse_cmdline(5, argv);

                    REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                    REQUIRE(parsed_command.error == ParseError::MISSING_VALUE);
                }
            }

            SECTION("curve")
            {
                SECTION("slot specified first")
                {
                    const char* argv[] = {"hsmctl", "generate-key", "--slot", "13", "--curve"};

                    auto parsed_command = parser.parse_cmdline(5, argv);

                    REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                    REQUIRE(parsed_command.error == ParseError::MISSING_VALUE);
                }

                SECTION("curve specified first")
                {
                    const char* argv[] = {"hsmctl", "generate-key", "--curve", "--slot", "26"};

                    auto parsed_command = parser.parse_cmdline(5, argv);

                    REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                    REQUIRE(parsed_command.error == ParseError::MISSING_VALUE);
                }
            }
        }

        SECTION("invalid value")
        {
            SECTION("slot")
            {
                SECTION("not a number")
                {
                    const char* argv[] = {"hsmctl", "generate-key", "--slot", "NaN"};

                    auto result = parser.parse_cmdline(4, argv);

                    REQUIRE(result.operation == Operation::GENERATE_KEY);
                    REQUIRE(result.error == ParseError::INVALID_VALUE);
                }

                SECTION("out of boundaries")
                {
                    const char* argv[] = {"hsmctl", "generate-key", "--slot", "32"};

                    auto result = parser.parse_cmdline(4, argv);

                    REQUIRE(result.operation == Operation::GENERATE_KEY);
                    REQUIRE(result.error == ParseError::INVALID_VALUE);
                }
            }

            SECTION("curve")
            {
                const char* argv[] = {"hsmctl", "generate-key", "--slot",
                                      "27",     "--curve",      "invalid_curve"};

                auto result = parser.parse_cmdline(6, argv);

                REQUIRE(result.operation == Operation::GENERATE_KEY);
                REQUIRE(result.error == ParseError::INVALID_VALUE);
            }
        }
    }
}
