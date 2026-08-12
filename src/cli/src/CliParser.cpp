#include <iostream>

#include "CliParser.hpp"

Command CliParser::parseCommand(int argc, const char* argv[])
{
    Command command;

    if (argc < 2)
    {
        command.error = ParseError::MISSING_OPERATION;
        return command;
    }

    std::string operation = std::string(argv[1]);

    if (operation == "--help")
    {
        command.operation = Operation::HELP;

        if (argc > 2)
        {
            command.error = ParseError::INVALID_OPTION;
        }

        return command;
    }

    if (operation == "status")
    {
        command.operation = Operation::STATUS;

        if (argc == 3 && std::string(argv[2]) == "--help")
        {
            command.requires_help = true;
            return command;
        }

        if (argc > 2)
        {
            command.error = ParseError::INVALID_OPTION;
        }

        return command;
    }

    if (operation == "logs")
    {
        command.operation = Operation::LOGS;

        if (argc == 3 && std::string(argv[2]) == "--help")
        {
            command.requires_help = true;
            return command;
        }

        if (argc > 2)
        {
            command.error = ParseError::INVALID_OPTION;
        }

        return command;
    }

    if (operation == "erase-key")
    {
        command.operation = Operation::ERASE_KEY;
        bool slot_option_used = false;

        if (argc < 3)
        {
            command.error = ParseError::MISSING_OPTION;
            return command;
        }

        for (int i = 2; i < argc; i++)
        {
            std::string arg = argv[i];

            if (arg == "--help")
            {
                if (argc != 3)
                {
                    command.error = ParseError::INVALID_OPTION;
                    return command;
                }

                command.requires_help = true;
                return command;
            }
            else if (arg == "--slot")
            {
                if (slot_option_used)
                {
                    command.error = ParseError::INVALID_OPTION;
                    return command;
                }

                slot_option_used = true;

                if (i + 1 >= argc)
                {
                    command.error = ParseError::MISSING_VALUE;
                    return command;
                }

                i++;

                try
                {
                    u_int8_t slot = std::stoi(argv[i]);

                    if (slot < MIN_SLOT || slot > MAX_SLOT)
                    {
                        command.error = ParseError::INVALID_VALUE;
                        return command;
                    }

                    command.options["slot"] = std::string(argv[i]);
                }
                catch (const std::invalid_argument&)
                {
                    command.error = ParseError::INVALID_VALUE;
                    return command;
                }
            }
            else
            {
                command.error = ParseError::INVALID_OPTION;
                return command;
            }
        }

        return command;
    }

    if (operation == "generate-key")
    {
        command.operation = Operation::GENERATE_KEY;
        bool slot_option_used = false;
        bool curve_option_used = false;
        command.options["curve"] = "ed25519";

        if (argc < 3)
        {
            command.error = ParseError::MISSING_OPTION;
            return command;
        }

        for (int i = 2; i < argc; i++)
        {
            std::string arg = argv[i];

            if (arg == "--help")
            {
                if (argc != 3)
                {
                    command.error = ParseError::INVALID_OPTION;
                    return command;
                }

                command.requires_help = true;
                return command;
            }
            else if (arg == "--slot")
            {
                if (slot_option_used)
                {
                    command.error = ParseError::INVALID_OPTION;
                    return command;
                }

                slot_option_used = true;

                // If the next command line argument does not exist or is an option
                if (i + 1 >= argc || std::string(argv[i + 1]).substr(0, 2) == "--")
                {
                    command.error = ParseError::MISSING_VALUE;
                    return command;
                }

                i++;

                try
                {
                    u_int8_t slot = std::stoi(argv[i]);

                    if (slot < MIN_SLOT || slot > MAX_SLOT)
                    {
                        command.error = ParseError::INVALID_VALUE;
                        return command;
                    }

                    command.options["slot"] = std::string(argv[i]);
                }
                catch (const std::invalid_argument&)
                {
                    command.error = ParseError::INVALID_VALUE;
                    return command;
                }
            }
            else if (arg == "--curve")
            {
                if (curve_option_used)
                {
                    command.error = ParseError::INVALID_OPTION;
                    return command;
                }

                curve_option_used = true;

                // If the next command line argument does not exist or is an option
                if (i + 1 >= argc || std::string(argv[i + 1]).substr(0, 2) == "--")
                {
                    command.error = ParseError::MISSING_VALUE;
                    return command;
                }

                i++;

                if (std::string(argv[i]) == "ed25519")
                {
                    command.options["curve"] = "ed25519";
                }
                else if (std::string(argv[i]) == "p256")
                {
                    command.options["curve"] = "p256";
                }
                else
                {
                    command.error = ParseError::INVALID_VALUE;
                    return command;
                }
            }
            else
            {
                command.error = ParseError::INVALID_OPTION;
                return command;
            }
        }

        if (slot_option_used == false)
        {
            command.error = ParseError::MISSING_OPTION;
            return command;
        }

        return command;
    }

    if (operation == "read-key")
    {
        command.operation = Operation::READ_KEY;
        bool slot_option_used = false;

        if (argc < 3)
        {
            command.error = ParseError::MISSING_OPTION;
            return command;
        }

        for (int i = 2; i < argc; i++)
        {
            std::string arg = argv[i];

            if (arg == "--help")
            {
                if (argc != 3)
                {
                    command.error = ParseError::INVALID_OPTION;
                    return command;
                }

                command.requires_help = true;
                return command;
            }
            else if (arg == "--slot")
            {
                if (slot_option_used)
                {
                    command.error = ParseError::INVALID_OPTION;
                    return command;
                }

                slot_option_used = true;

                if (i + 1 >= argc)
                {
                    command.error = ParseError::MISSING_VALUE;
                    return command;
                }

                i++;

                try
                {
                    u_int8_t slot = std::stoi(argv[i]);

                    if (slot < MIN_SLOT || slot > MAX_SLOT)
                    {
                        command.error = ParseError::INVALID_VALUE;
                        return command;
                    }

                    command.options["slot"] = std::string(argv[i]);
                }
                catch (const std::invalid_argument&)
                {
                    command.error = ParseError::INVALID_VALUE;
                    return command;
                }
            }
            else
            {
                command.error = ParseError::INVALID_OPTION;
                return command;
            }
        }

        return command;
    }

    command.error = ParseError::INVALID_OPERATION;

    return command;
}