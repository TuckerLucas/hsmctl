#include "cli_parser.hpp"

#include <iostream>

Command cli_parser::parse_cmdline(int argc, const char* argv[])
{
    if (argc < 2)
    {
        return Command::UNKNOWN;
    }

    std::string command = std::string(argv[1]);

    if (command == "--help")
    {
        return Command::HELP;
    }

    if (command == "inject-key")
    {
        return Command::INJECT_KEY;
    }

    if (command == "update-key")
    {
        return Command::UPDATE_KEY;
    }

    if (command == "get-key")
    {
        return Command::GET_KEY;
    }

    if (command == "erase-key")
    {
        return Command::ERASE_KEY;
    }

    return Command::UNKNOWN;
}