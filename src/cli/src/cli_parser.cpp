#include "cli_parser.hpp"

#include <iostream>

Command cli_parser::parse_cmdline(int argc, const char* argv[])
{
    if (argc < 2)
    {
        return Command::UNKNOWN;
    }

    if (std::string(argv[1]) == "--help")
    {
        return Command::HELP;
    }

    if (std::string(argv[1]) == "inject-key")
    {
        return Command::INJECT_KEY;
    }

    if (std::string(argv[1]) == "update-key")
    {
        return Command::UPDATE_KEY;
    }

    if (std::string(argv[1]) == "get-key")
    {
        return Command::GET_KEY;
    }

    if (std::string(argv[1]) == "erase-key")
    {
        return Command::ERASE_KEY;
    }

    return Command::UNKNOWN;
}