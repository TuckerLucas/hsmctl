#include "cli_parser.hpp"

#include <iostream>

Command cli_parser::parse_cmdline(int argc, const char* argv[])
{
    std::string command = std::string(argv[1]);

    if (command == "--help")
    {
        return Command::HELP;
    }

    if (command == "status")
    {
        return Command::STATUS;
    }

    return Command::UNKNOWN;
}