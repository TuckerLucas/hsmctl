#include "cli_parser.hpp"

#include <iostream>

Command cli_parser::parse_cmdline(int argc, const char* argv[])
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
        return command;
    }

    if (operation == "status")
    {
        command.operation = Operation::STATUS;
        return command;
    }

    if (operation == "erase-key")
    {
        command.operation = Operation::ERASE_KEY;

        if (argc < 3)
        {
            command.error = ParseError::MISSING_OPTION;
            return command;
        }

        std::string option = std::string(argv[2]);

        if (option != "--slot")
        {
            command.error = ParseError::INVALID_OPTION;
            return command;
        }

        if (argc < 4)
        {
            command.error = ParseError::MISSING_VALUE;
            return command;
        }

        try
        {
            u_int8_t slot = std::stoi(std::string(argv[3]));

            if (slot < 0 || slot > 31)
            {
                command.error = ParseError::INVALID_VALUE;
                return command;
            }

            command.options["slot"] = std::string(argv[3]);
        }
        catch (const std::invalid_argument&)
        {
            command.error = ParseError::INVALID_VALUE;
            return command;
        }

        return command;
    }

    return command;
}