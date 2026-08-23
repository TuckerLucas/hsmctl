#include "CliParser.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string>

TEST_CASE("CliParser parseCommand")
{
    CliParser parser;

    SECTION("no operation")
    {
        const char* argv[] = {"hsmctl"};

        auto parsed_command = parser.parseCommand(std::size(argv), argv);

        REQUIRE(parsed_command.operation == Operation::NONE);
        REQUIRE(parsed_command.error == ParseError::MISSING_OPERATION);
    }

    SECTION("invalid operation")
    {
        const char* argv[] = {"hsmctl", "invalid-operation"};

        auto result = parser.parseCommand(std::size(argv), argv);

        REQUIRE(result.operation == Operation::NONE);
        REQUIRE(result.error == ParseError::INVALID_OPERATION);
    }

    SECTION("help")
    {
        SECTION("success")
        {
            const char* argv[] = {"hsmctl", "--help"};

            auto parsed_command = parser.parseCommand(std::size(argv), argv);

            REQUIRE(parsed_command.operation == Operation::HELP);
            REQUIRE(parsed_command.error == ParseError::NONE);
        }

        SECTION("invalid option")
        {
            const char* argv[] = {"hsmctl", "--help", "--some-option"};

            auto parsed_command = parser.parseCommand(std::size(argv), argv);

            REQUIRE(parsed_command.operation == Operation::HELP);
            REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
        }
    }

    SECTION("status")
    {
        SECTION("success")
        {
            const char* argv[] = {"hsmctl", "status"};

            auto parsed_command = parser.parseCommand(std::size(argv), argv);

            REQUIRE(parsed_command.operation == Operation::STATUS);
            REQUIRE(parsed_command.error == ParseError::NONE);
            REQUIRE(parsed_command.requires_help == false);
        }

        SECTION("help")
        {
            const char* argv[] = {"hsmctl", "status", "--help"};

            auto parsed_command = parser.parseCommand(std::size(argv), argv);

            REQUIRE(parsed_command.operation == Operation::STATUS);
            REQUIRE(parsed_command.error == ParseError::NONE);
            REQUIRE(parsed_command.requires_help == true);
        }

        SECTION("invalid option")
        {
            const char* argv[] = {"hsmctl", "status", "--some-option"};

            auto parsed_command = parser.parseCommand(std::size(argv), argv);

            REQUIRE(parsed_command.operation == Operation::STATUS);
            REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
        }
    }

    SECTION("logs")
    {
        SECTION("success")
        {
            const char* argv[] = {"hsmctl", "logs"};

            auto parsed_command = parser.parseCommand(std::size(argv), argv);

            REQUIRE(parsed_command.operation == Operation::LOGS);
            REQUIRE(parsed_command.error == ParseError::NONE);
            REQUIRE(parsed_command.requires_help == false);
        }

        SECTION("help")
        {
            const char* argv[] = {"hsmctl", "logs", "--help"};

            auto parsed_command = parser.parseCommand(std::size(argv), argv);

            REQUIRE(parsed_command.operation == Operation::LOGS);
            REQUIRE(parsed_command.error == ParseError::NONE);
            REQUIRE(parsed_command.requires_help == true);
        }

        SECTION("invalid option")
        {
            const char* argv[] = {"hsmctl", "logs", "--some-option"};

            auto parsed_command = parser.parseCommand(std::size(argv), argv);

            REQUIRE(parsed_command.operation == Operation::LOGS);
            REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
        }
    }

    SECTION("erase key")
    {
        SECTION("success")
        {
            SECTION("non-boundary slot")
            {
                const char* argv[] = {"hsmctl", "erase-key", "--slot", "2"};

                auto parsed_command = parser.parseCommand(std::size(argv), argv);

                REQUIRE(parsed_command.operation == Operation::ERASE_KEY);
                REQUIRE(parsed_command.options["slot"] == "2");
                REQUIRE(parsed_command.error == ParseError::NONE);
                REQUIRE(parsed_command.requires_help == false);
            }

            SECTION("boundary slots")
            {
                SECTION("lower boundary")
                {
                    std::string min_slot_str = std::to_string(MIN_SLOT);
                    const char* argv[] = {"hsmctl", "erase-key", "--slot", min_slot_str.c_str()};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::ERASE_KEY);
                    REQUIRE(parsed_command.options["slot"] == min_slot_str.c_str());
                    REQUIRE(parsed_command.error == ParseError::NONE);
                    REQUIRE(parsed_command.requires_help == false);
                }

                SECTION("upper boundary")
                {
                    std::string max_slot_str = std::to_string(MAX_SLOT);
                    const char* argv[] = {"hsmctl", "erase-key", "--slot", max_slot_str.c_str()};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::ERASE_KEY);
                    REQUIRE(parsed_command.options["slot"] == max_slot_str.c_str());
                    REQUIRE(parsed_command.error == ParseError::NONE);
                    REQUIRE(parsed_command.requires_help == false);
                }
            }
        }

        SECTION("help")
        {
            const char* argv[] = {"hsmctl", "erase-key", "--help"};

            auto parsed_command = parser.parseCommand(std::size(argv), argv);

            REQUIRE(parsed_command.operation == Operation::ERASE_KEY);
            REQUIRE(parsed_command.error == ParseError::NONE);
            REQUIRE(parsed_command.requires_help);
        }

        SECTION("missing option")
        {
            const char* argv[] = {"hsmctl", "erase-key"};

            auto parsed_command = parser.parseCommand(std::size(argv), argv);

            REQUIRE(parsed_command.operation == Operation::ERASE_KEY);
            REQUIRE(parsed_command.error == ParseError::MISSING_OPTION);
        }

        SECTION("invalid option")
        {
            SECTION("unknown option")
            {
                const char* argv[] = {"hsmctl", "erase-key", "--invalid-option"};

                auto parsed_command = parser.parseCommand(std::size(argv), argv);

                REQUIRE(parsed_command.operation == Operation::ERASE_KEY);
                REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
            }
            SECTION("trailing garbage after valid command")
            {
                const char* argv[] = {"hsmctl", "erase-key", "--slot", "0", "garbage"};

                auto parsed_command = parser.parseCommand(std::size(argv), argv);

                REQUIRE(parsed_command.operation == Operation::ERASE_KEY);
                REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
            }

            SECTION("duplicate slot")
            {
                const char* argv[] = {"hsmctl", "erase-key", "--slot", "3", "--slot", "14"};

                auto parsed_command = parser.parseCommand(std::size(argv), argv);

                REQUIRE(parsed_command.operation == Operation::ERASE_KEY);
                REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
            }
        }

        SECTION("missing slot value")
        {
            const char* argv[] = {"hsmctl", "erase-key", "--slot"};

            auto parsed_command = parser.parseCommand(std::size(argv), argv);

            REQUIRE(parsed_command.operation == Operation::ERASE_KEY);
            REQUIRE(parsed_command.error == ParseError::MISSING_VALUE);
        }

        SECTION("invalid slot value")
        {
            SECTION("not a number")
            {
                const char* argv[] = {"hsmctl", "erase-key", "--slot", "NaN"};

                auto result = parser.parseCommand(std::size(argv), argv);

                REQUIRE(result.operation == Operation::ERASE_KEY);
                REQUIRE(result.error == ParseError::INVALID_VALUE);
            }

            SECTION("out of boundaries")
            {
                const char* argv[] = {"hsmctl", "erase-key", "--slot", "32"};

                auto result = parser.parseCommand(std::size(argv), argv);

                REQUIRE(result.operation == Operation::ERASE_KEY);
                REQUIRE(result.error == ParseError::INVALID_VALUE);
            }
        }
    }

    SECTION("generate key")
    {
        SECTION("success")
        {
            SECTION("unspecified curve")
            {
                const char* argv[] = {"hsmctl", "generate-key", "--slot", "10"};

                auto parsed_command = parser.parseCommand(std::size(argv), argv);

                REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                REQUIRE(parsed_command.options["slot"] == "10");
                REQUIRE(parsed_command.options["curve"] == "ed25519");
                REQUIRE(parsed_command.error == ParseError::NONE);
                REQUIRE(parsed_command.requires_help == false);
            }

            SECTION("specified curve")
            {
                SECTION("Ed25519")
                {
                    const char* argv[] = {"hsmctl", "generate-key", "--slot",
                                          "12",     "--curve",      "ed25519"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                    REQUIRE(parsed_command.options["slot"] == "12");
                    REQUIRE(parsed_command.options["curve"] == "ed25519");
                    REQUIRE(parsed_command.error == ParseError::NONE);
                    REQUIRE(parsed_command.requires_help == false);
                }

                SECTION("P-256")
                {
                    const char* argv[] = {"hsmctl", "generate-key", "--slot",
                                          "18",     "--curve",      "p256"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                    REQUIRE(parsed_command.options["slot"] == "18");
                    REQUIRE(parsed_command.options["curve"] == "p256");
                    REQUIRE(parsed_command.error == ParseError::NONE);
                    REQUIRE(parsed_command.requires_help == false);
                }

                SECTION("reversed operation order")
                {
                    const char* argv[] = {"hsmctl", "generate-key", "--curve",
                                          "p256",   "--slot",       "16"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                    REQUIRE(parsed_command.options["slot"] == "16");
                    REQUIRE(parsed_command.options["curve"] == "p256");
                    REQUIRE(parsed_command.error == ParseError::NONE);
                    REQUIRE(parsed_command.requires_help == false);
                }
            }

            SECTION("boundary slots")
            {
                SECTION("lower boundary")
                {
                    std::string min_slot_str = std::to_string(MIN_SLOT);
                    const char* argv[] = {"hsmctl", "generate-key", "--slot", min_slot_str.c_str()};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                    REQUIRE(parsed_command.options["slot"] == min_slot_str.c_str());
                    REQUIRE(parsed_command.error == ParseError::NONE);
                    REQUIRE(parsed_command.requires_help == false);
                }

                SECTION("upper boundary")
                {
                    std::string max_slot_str = std::to_string(MAX_SLOT);
                    const char* argv[] = {"hsmctl", "generate-key", "--slot", max_slot_str.c_str()};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                    REQUIRE(parsed_command.options["slot"] == max_slot_str.c_str());
                    REQUIRE(parsed_command.error == ParseError::NONE);
                    REQUIRE(parsed_command.requires_help == false);
                }
            }
        }

        SECTION("help")
        {
            const char* argv[] = {"hsmctl", "generate-key", "--help"};

            auto parsed_command = parser.parseCommand(std::size(argv), argv);

            REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
            REQUIRE(parsed_command.error == ParseError::NONE);
            REQUIRE(parsed_command.requires_help);
        }

        SECTION("missing option")
        {
            SECTION("unspecified curve")
            {
                const char* argv[] = {"hsmctl", "generate-key"};

                auto parsed_command = parser.parseCommand(std::size(argv), argv);

                REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                REQUIRE(parsed_command.error == ParseError::MISSING_OPTION);
            }

            SECTION("specified curve")
            {
                const char* argv[] = {"hsmctl", "generate-key", "--curve", "p256"};

                auto parsed_command = parser.parseCommand(std::size(argv), argv);

                REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                REQUIRE(parsed_command.error == ParseError::MISSING_OPTION);
            }
        }

        SECTION("invalid option")
        {
            SECTION("unknown option")
            {
                const char* argv[] = {"hsmctl", "generate-key", "--invalid-option"};

                auto parsed_command = parser.parseCommand(std::size(argv), argv);

                REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
            }

            SECTION("trailing garbage after valid command")
            {
                const char* argv[] = {"hsmctl", "generate-key", "--slot", "0", "garbage"};

                auto parsed_command = parser.parseCommand(std::size(argv), argv);

                REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
            }

            SECTION("duplicate options")
            {
                SECTION("slot")
                {
                    const char* argv[] = {"hsmctl", "generate-key", "--slot", "3", "--slot", "14"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                    REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
                }

                SECTION("curve")
                {
                    const char* argv[] = {"hsmctl",  "generate-key", "--slot",  "3",
                                          "--curve", "ed25519",      "--curve", "p256"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

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

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                    REQUIRE(parsed_command.error == ParseError::MISSING_VALUE);
                }

                SECTION("slot specified first")
                {
                    const char* argv[] = {"hsmctl", "generate-key", "--slot", "--curve", "p256"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                    REQUIRE(parsed_command.error == ParseError::MISSING_VALUE);
                }

                SECTION("curve specified first")
                {
                    const char* argv[] = {"hsmctl", "generate-key", "--curve", "p256", "--slot"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                    REQUIRE(parsed_command.error == ParseError::MISSING_VALUE);
                }
            }

            SECTION("curve")
            {
                SECTION("slot specified first")
                {
                    const char* argv[] = {"hsmctl", "generate-key", "--slot", "13", "--curve"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::GENERATE_KEY);
                    REQUIRE(parsed_command.error == ParseError::MISSING_VALUE);
                }

                SECTION("curve specified first")
                {
                    const char* argv[] = {"hsmctl", "generate-key", "--curve", "--slot", "26"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

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

                    auto result = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(result.operation == Operation::GENERATE_KEY);
                    REQUIRE(result.error == ParseError::INVALID_VALUE);
                }

                SECTION("out of boundaries")
                {
                    const char* argv[] = {"hsmctl", "generate-key", "--slot", "32"};

                    auto result = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(result.operation == Operation::GENERATE_KEY);
                    REQUIRE(result.error == ParseError::INVALID_VALUE);
                }
            }

            SECTION("curve")
            {
                const char* argv[] = {"hsmctl", "generate-key", "--slot",
                                      "27",     "--curve",      "invalid_curve"};

                auto result = parser.parseCommand(std::size(argv), argv);

                REQUIRE(result.operation == Operation::GENERATE_KEY);
                REQUIRE(result.error == ParseError::INVALID_VALUE);
            }
        }
    }

    SECTION("read key")
    {
        SECTION("success")
        {
            SECTION("non-boundary slot")
            {
                const char* argv[] = {"hsmctl", "read-key", "--slot", "2"};

                auto parsed_command = parser.parseCommand(std::size(argv), argv);

                REQUIRE(parsed_command.operation == Operation::READ_KEY);
                REQUIRE(parsed_command.options["slot"] == "2");
                REQUIRE(parsed_command.error == ParseError::NONE);
                REQUIRE(parsed_command.requires_help == false);
            }

            SECTION("boundary slots")
            {
                SECTION("lower boundary")
                {
                    std::string min_slot_str = std::to_string(MIN_SLOT);
                    const char* argv[] = {"hsmctl", "read-key", "--slot", min_slot_str.c_str()};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::READ_KEY);
                    REQUIRE(parsed_command.options["slot"] == min_slot_str.c_str());
                    REQUIRE(parsed_command.error == ParseError::NONE);
                    REQUIRE(parsed_command.requires_help == false);
                }

                SECTION("upper boundary")
                {
                    std::string max_slot_str = std::to_string(MAX_SLOT);
                    const char* argv[] = {"hsmctl", "read-key", "--slot", max_slot_str.c_str()};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::READ_KEY);
                    REQUIRE(parsed_command.options["slot"] == max_slot_str.c_str());
                    REQUIRE(parsed_command.error == ParseError::NONE);
                    REQUIRE(parsed_command.requires_help == false);
                }
            }
        }

        SECTION("help")
        {
            const char* argv[] = {"hsmctl", "read-key", "--help"};

            auto parsed_command = parser.parseCommand(std::size(argv), argv);

            REQUIRE(parsed_command.operation == Operation::READ_KEY);
            REQUIRE(parsed_command.error == ParseError::NONE);
            REQUIRE(parsed_command.requires_help);
        }

        SECTION("missing option")
        {
            const char* argv[] = {"hsmctl", "read-key"};

            auto parsed_command = parser.parseCommand(std::size(argv), argv);

            REQUIRE(parsed_command.operation == Operation::READ_KEY);
            REQUIRE(parsed_command.error == ParseError::MISSING_OPTION);
        }

        SECTION("invalid option")
        {
            SECTION("unknown option")
            {
                const char* argv[] = {"hsmctl", "read-key", "--invalid-option"};

                auto parsed_command = parser.parseCommand(std::size(argv), argv);

                REQUIRE(parsed_command.operation == Operation::READ_KEY);
                REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
            }

            SECTION("trailing garbage after valid command")
            {
                const char* argv[] = {"hsmctl", "read-key", "--slot", "0", "garbage"};

                auto parsed_command = parser.parseCommand(std::size(argv), argv);

                REQUIRE(parsed_command.operation == Operation::READ_KEY);
                REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
            }

            SECTION("duplicate slot")
            {
                const char* argv[] = {"hsmctl", "read-key", "--slot", "3", "--slot", "14"};

                auto parsed_command = parser.parseCommand(std::size(argv), argv);

                REQUIRE(parsed_command.operation == Operation::READ_KEY);
                REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
            }
        }

        SECTION("missing slot value")
        {
            const char* argv[] = {"hsmctl", "read-key", "--slot"};

            auto parsed_command = parser.parseCommand(std::size(argv), argv);

            REQUIRE(parsed_command.operation == Operation::READ_KEY);
            REQUIRE(parsed_command.error == ParseError::MISSING_VALUE);
        }

        SECTION("invalid slot value")
        {
            SECTION("not a number")
            {
                const char* argv[] = {"hsmctl", "read-key", "--slot", "NaN"};

                auto result = parser.parseCommand(std::size(argv), argv);

                REQUIRE(result.operation == Operation::READ_KEY);
                REQUIRE(result.error == ParseError::INVALID_VALUE);
            }

            SECTION("out of boundaries")
            {
                const char* argv[] = {"hsmctl", "read-key", "--slot", "32"};

                auto result = parser.parseCommand(std::size(argv), argv);

                REQUIRE(result.operation == Operation::READ_KEY);
                REQUIRE(result.error == ParseError::INVALID_VALUE);
            }
        }
    }

    SECTION("list all keys")
    {
        SECTION("success")
        {
            SECTION("unspecified verbosity")
            {
                const char* argv[] = {"hsmctl", "list-keys"};

                auto parsed_command = parser.parseCommand(std::size(argv), argv);

                REQUIRE(parsed_command.operation == Operation::LIST_KEYS);
                REQUIRE(parsed_command.error == ParseError::NONE);
                REQUIRE(parsed_command.requires_help == false);
            }

            SECTION("specified verbosity")
            {
                const char* argv[] = {"hsmctl", "list-keys", "--verbose"};

                auto parsed_command = parser.parseCommand(std::size(argv), argv);

                REQUIRE(parsed_command.operation == Operation::LIST_KEYS);
                REQUIRE(parsed_command.requires_help == false);
                REQUIRE(parsed_command.options["verbose"] == "true");
                REQUIRE(parsed_command.error == ParseError::NONE);
            }
        }

        SECTION("help")
        {
            const char* argv[] = {"hsmctl", "list-keys", "--help"};

            auto parsed_command = parser.parseCommand(std::size(argv), argv);

            REQUIRE(parsed_command.operation == Operation::LIST_KEYS);
            REQUIRE(parsed_command.error == ParseError::NONE);
            REQUIRE(parsed_command.requires_help);
        }

        SECTION("invalid option")
        {
            SECTION("unknown option")
            {
                const char* argv[] = {"hsmctl", "list-keys", "--some-option"};

                auto parsed_command = parser.parseCommand(std::size(argv), argv);

                REQUIRE(parsed_command.operation == Operation::LIST_KEYS);
                REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
            }

            SECTION("trailing garbage after valid command")
            {
                const char* argv[] = {"hsmctl", "list-keys", "--verbose", "garbage"};

                auto parsed_command = parser.parseCommand(std::size(argv), argv);

                REQUIRE(parsed_command.operation == Operation::LIST_KEYS);
                REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
            }
        }
    }

    SECTION("sign")
    {
        SECTION("success")
        {
            SECTION("data")
            {
                SECTION("slot specified first")
                {
                    const char* argv[] = {"hsmctl", "sign", "--slot", "3", "--data", "data"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::SIGN);
                    REQUIRE(parsed_command.error == ParseError::NONE);
                    REQUIRE(parsed_command.requires_help == false);
                    REQUIRE(parsed_command.options["slot"] == "3");
                    REQUIRE(parsed_command.options["data"] == "data");
                    REQUIRE(parsed_command.options["file"].empty());
                }

                SECTION("slot specified last")
                {
                    const char* argv[] = {"hsmctl", "sign", "--data", "data", "--slot", "3"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::SIGN);
                    REQUIRE(parsed_command.error == ParseError::NONE);
                    REQUIRE(parsed_command.requires_help == false);
                    REQUIRE(parsed_command.options["data"] == "data");
                    REQUIRE(parsed_command.options["slot"] == "3");
                    REQUIRE(parsed_command.options["file"].empty());
                }
            }

            SECTION("file")
            {
                SECTION("slot specified first")
                {
                    const char* argv[] = {"hsmctl", "sign", "--slot", "3", "--file", "file.pdf"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::SIGN);
                    REQUIRE(parsed_command.error == ParseError::NONE);
                    REQUIRE(parsed_command.requires_help == false);
                    REQUIRE(parsed_command.options["slot"] == "3");
                    REQUIRE(parsed_command.options["file"] == "file.pdf");
                    REQUIRE(parsed_command.options["data"].empty());
                }

                SECTION("slot specified last")
                {
                    const char* argv[] = {"hsmctl", "sign", "--file", "file.pdf", "--slot", "3"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::SIGN);
                    REQUIRE(parsed_command.error == ParseError::NONE);
                    REQUIRE(parsed_command.requires_help == false);
                    REQUIRE(parsed_command.options["file"] == "file.pdf");
                    REQUIRE(parsed_command.options["slot"] == "3");
                    REQUIRE(parsed_command.options["data"].empty());
                }
            }

            SECTION("boundary slots")
            {
                SECTION("lower boundary")
                {
                    const char* argv[] = {
                        "hsmctl", "sign", "--slot", "0", "--file", "file.pdf",
                    };

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::SIGN);
                    REQUIRE(parsed_command.error == ParseError::NONE);
                    REQUIRE(parsed_command.requires_help == false);
                    REQUIRE(parsed_command.options["slot"] == "0");
                    REQUIRE(parsed_command.options["file"] == "file.pdf");
                    REQUIRE(parsed_command.options["data"].empty());
                }

                SECTION("upper boundary")
                {
                    const char* argv[] = {
                        "hsmctl", "sign", "--slot", "31", "--data", "data",
                    };

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::SIGN);
                    REQUIRE(parsed_command.error == ParseError::NONE);
                    REQUIRE(parsed_command.requires_help == false);
                    REQUIRE(parsed_command.options["slot"] == "31");
                    REQUIRE(parsed_command.options["data"] == "data");
                    REQUIRE(parsed_command.options["file"].empty());
                }
            }
        }

        SECTION("help")
        {
            SECTION("success")
            {
                const char* argv[] = {"hsmctl", "sign", "--help"};

                auto parsed_command = parser.parseCommand(std::size(argv), argv);

                REQUIRE(parsed_command.operation == Operation::SIGN);
                REQUIRE(parsed_command.error == ParseError::NONE);
                REQUIRE(parsed_command.requires_help);
            }

            SECTION("trailing garbage")
            {
                const char* argv[] = {"hsmctl", "sign", "--help", "garbage"};

                auto parsed_command = parser.parseCommand(std::size(argv), argv);

                REQUIRE(parsed_command.operation == Operation::SIGN);
                REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
                REQUIRE(parsed_command.requires_help == false);
            }
        }

        SECTION("missing option")
        {
            SECTION("unspecified slot and data/file")
            {
                const char* argv[] = {"hsmctl", "sign"};

                auto parsed_command = parser.parseCommand(std::size(argv), argv);

                REQUIRE(parsed_command.operation == Operation::SIGN);
                REQUIRE(parsed_command.error == ParseError::MISSING_OPTION);
            }

            SECTION("unspecified slot")
            {
                SECTION("specified data")
                {
                    const char* argv[] = {"hsmctl", "sign", "--data", "data"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::SIGN);
                    REQUIRE(parsed_command.error == ParseError::MISSING_OPTION);
                    REQUIRE(parsed_command.options["data"] == "data");
                    REQUIRE(parsed_command.options["slot"].empty());
                }

                SECTION("specified file")
                {
                    const char* argv[] = {"hsmctl", "sign", "--file", "file.pdf"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::SIGN);
                    REQUIRE(parsed_command.error == ParseError::MISSING_OPTION);
                    REQUIRE(parsed_command.options["file"] == "file.pdf");
                    REQUIRE(parsed_command.options["slot"].empty());
                }
            }

            SECTION("unspecified data/file")
            {
                const char* argv[] = {"hsmctl", "sign", "--slot", "10"};

                auto parsed_command = parser.parseCommand(std::size(argv), argv);

                REQUIRE(parsed_command.operation == Operation::SIGN);
                REQUIRE(parsed_command.error == ParseError::MISSING_OPTION);
                REQUIRE(parsed_command.options["slot"] == "10");
            }
        }

        SECTION("invalid option")
        {
            SECTION("unknown option")
            {
                const char* argv[] = {"hsmctl", "sign", "--invalid-option"};

                auto parsed_command = parser.parseCommand(std::size(argv), argv);

                REQUIRE(parsed_command.operation == Operation::SIGN);
                REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
            }

            SECTION("duplicate options")
            {
                SECTION("slot")
                {
                    const char* argv[] = {"hsmctl", "sign", "--slot", "26", "--slot"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::SIGN);
                    REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
                    REQUIRE(parsed_command.options["slot"] == "26");
                }

                SECTION("data")
                {
                    const char* argv[] = {"hsmctl", "sign",  "--slot", "26",
                                          "--data", "data1", "--data", "data2"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::SIGN);
                    REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
                    REQUIRE(parsed_command.options["slot"] == "26");
                    REQUIRE(parsed_command.options["data"] == "data1");
                }

                SECTION("file")
                {
                    const char* argv[] = {"hsmctl", "sign",      "--slot", "26",
                                          "--file", "file1.pdf", "--file", "file2.pdf"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::SIGN);
                    REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
                    REQUIRE(parsed_command.options["slot"] == "26");
                    REQUIRE(parsed_command.options["file"] == "file1.pdf");
                }

                SECTION("data and file used in same command")
                {
                    SECTION("data specified first")
                    {
                        const char* argv[] = {"hsmctl", "sign", "--slot", "26",
                                              "--data", "data", "--file", "file.pdf"};

                        auto parsed_command = parser.parseCommand(std::size(argv), argv);

                        REQUIRE(parsed_command.operation == Operation::SIGN);
                        REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
                        REQUIRE(parsed_command.options["slot"] == "26");
                        REQUIRE(parsed_command.options["data"] == "data");
                        REQUIRE(parsed_command.options["file"].empty());
                    }

                    SECTION("file specified first")
                    {
                        const char* argv[] = {"hsmctl", "sign",     "--slot", "26",
                                              "--file", "file.pdf", "--data", "data"};

                        auto parsed_command = parser.parseCommand(std::size(argv), argv);

                        REQUIRE(parsed_command.operation == Operation::SIGN);
                        REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
                        REQUIRE(parsed_command.options["slot"] == "26");
                        REQUIRE(parsed_command.options["file"] == "file.pdf");
                        REQUIRE(parsed_command.options["data"].empty());
                    }
                }
            }

            SECTION("trailing garbage after valid command")
            {
                SECTION("data")
                {
                    SECTION("slot specified first")
                    {
                        const char* argv[] = {"hsmctl", "sign", "--slot", "26",
                                              "--data", "data", "garbage"};

                        auto parsed_command = parser.parseCommand(std::size(argv), argv);

                        REQUIRE(parsed_command.operation == Operation::SIGN);
                        REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
                        REQUIRE(parsed_command.options["slot"] == "26");
                        REQUIRE(parsed_command.options["data"] == "data");
                    }

                    SECTION("slot specified last")
                    {
                        const char* argv[] = {"hsmctl", "sign", "--data", "data",
                                              "--slot", "26",   "garbage"};

                        auto parsed_command = parser.parseCommand(std::size(argv), argv);

                        REQUIRE(parsed_command.operation == Operation::SIGN);
                        REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
                        REQUIRE(parsed_command.options["data"] == "data");
                        REQUIRE(parsed_command.options["slot"] == "26");
                    }
                }

                SECTION("file")
                {
                    SECTION("slot specified first")
                    {
                        const char* argv[] = {"hsmctl", "sign",     "--slot", "26",
                                              "--file", "file.pdf", "garbage"};

                        auto parsed_command = parser.parseCommand(std::size(argv), argv);

                        REQUIRE(parsed_command.operation == Operation::SIGN);
                        REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
                        REQUIRE(parsed_command.options["slot"] == "26");
                        REQUIRE(parsed_command.options["file"] == "file.pdf");
                    }

                    SECTION("slot specified last")
                    {
                        const char* argv[] = {"hsmctl", "sign", "--file", "file.pdf",
                                              "--slot", "26",   "garbage"};

                        auto parsed_command = parser.parseCommand(std::size(argv), argv);

                        REQUIRE(parsed_command.operation == Operation::SIGN);
                        REQUIRE(parsed_command.error == ParseError::INVALID_OPTION);
                        REQUIRE(parsed_command.options["file"] == "file.pdf");
                        REQUIRE(parsed_command.options["slot"] == "26");
                    }
                }
            }
        }

        SECTION("missing value")
        {
            SECTION("slot")
            {
                SECTION("unspecified data/file")
                {
                    const char* argv[] = {"hsmctl", "sign", "--slot"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::SIGN);
                    REQUIRE(parsed_command.error == ParseError::MISSING_VALUE);
                    REQUIRE(parsed_command.options["slot"].empty());
                }

                SECTION("data specified")
                {
                    const char* argv[] = {"hsmctl", "sign", "--slot", "--data", "data"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::SIGN);
                    REQUIRE(parsed_command.error == ParseError::MISSING_VALUE);
                    REQUIRE(parsed_command.options["slot"].empty());
                    REQUIRE(parsed_command.options["data"].empty());
                }

                SECTION("file specified")
                {
                    const char* argv[] = {"hsmctl", "sign", "--slot", "--file", "file.pdf"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::SIGN);
                    REQUIRE(parsed_command.error == ParseError::MISSING_VALUE);
                    REQUIRE(parsed_command.options["slot"].empty());
                    REQUIRE(parsed_command.options["file"].empty());
                }

                SECTION("data specified first")
                {
                    const char* argv[] = {"hsmctl", "sign", "--data", "data", "--slot"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::SIGN);
                    REQUIRE(parsed_command.error == ParseError::MISSING_VALUE);
                    REQUIRE(parsed_command.options["data"] == "data");
                    REQUIRE(parsed_command.options["slot"].empty());
                }

                SECTION("file specified first")
                {
                    const char* argv[] = {"hsmctl", "sign", "--file", "file.pdf", "--slot"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::SIGN);
                    REQUIRE(parsed_command.error == ParseError::MISSING_VALUE);
                    REQUIRE(parsed_command.options["file"] == "file.pdf");
                    REQUIRE(parsed_command.options["slot"].empty());
                }
            }

            SECTION("data")
            {
                SECTION("unspecified slot")
                {
                    const char* argv[] = {"hsmctl", "sign", "--data"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::SIGN);
                    REQUIRE(parsed_command.error == ParseError::MISSING_VALUE);
                    REQUIRE(parsed_command.options["data"].empty());
                    REQUIRE(parsed_command.options["slot"].empty());
                }

                SECTION("slot specified first")
                {
                    const char* argv[] = {"hsmctl", "sign", "--slot", "12", "--data"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::SIGN);
                    REQUIRE(parsed_command.error == ParseError::MISSING_VALUE);
                    REQUIRE(parsed_command.options["slot"] == "12");
                    REQUIRE(parsed_command.options["data"].empty());
                }

                SECTION("slot specified last")
                {
                    const char* argv[] = {"hsmctl", "sign", "--data", "--slot", "12"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::SIGN);
                    REQUIRE(parsed_command.error == ParseError::MISSING_VALUE);
                    REQUIRE(parsed_command.options["data"].empty());
                    REQUIRE(parsed_command.options["slot"].empty());
                }
            }

            SECTION("file")
            {
                SECTION("unspecified slot")
                {
                    const char* argv[] = {"hsmctl", "sign", "--file"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::SIGN);
                    REQUIRE(parsed_command.error == ParseError::MISSING_VALUE);
                    REQUIRE(parsed_command.options["slot"].empty());
                    REQUIRE(parsed_command.options["file"].empty());
                }

                SECTION("slot specified first")
                {
                    const char* argv[] = {"hsmctl", "sign", "--slot", "15", "--file"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::SIGN);
                    REQUIRE(parsed_command.error == ParseError::MISSING_VALUE);
                    REQUIRE(parsed_command.options["slot"] == "15");
                    REQUIRE(parsed_command.options["file"].empty());
                }

                SECTION("slot specified last")
                {
                    const char* argv[] = {"hsmctl", "sign", "--file", "--slot", "15"};

                    auto parsed_command = parser.parseCommand(std::size(argv), argv);

                    REQUIRE(parsed_command.operation == Operation::SIGN);
                    REQUIRE(parsed_command.error == ParseError::MISSING_VALUE);
                    REQUIRE(parsed_command.options["file"].empty());
                    REQUIRE(parsed_command.options["slot"].empty());
                }
            }
        }

        SECTION("invalid value")
        {
            SECTION("out of boundaries")
            {
                const char* argv[] = {"hsmctl", "sign", "--slot", "32"};

                auto parsed_command = parser.parseCommand(std::size(argv), argv);

                REQUIRE(parsed_command.operation == Operation::SIGN);
                REQUIRE(parsed_command.error == ParseError::INVALID_VALUE);
                REQUIRE(parsed_command.options["slot"].empty());
            }

            SECTION("not a number")
            {
                const char* argv[] = {"hsmctl", "sign", "--slot", "NaN"};

                auto parsed_command = parser.parseCommand(std::size(argv), argv);

                REQUIRE(parsed_command.operation == Operation::SIGN);
                REQUIRE(parsed_command.error == ParseError::INVALID_VALUE);
                REQUIRE(parsed_command.options["slot"].empty());
            }
        }
    }
}
