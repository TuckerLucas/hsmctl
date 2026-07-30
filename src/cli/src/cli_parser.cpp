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

    return Command::UNKNOWN;
}