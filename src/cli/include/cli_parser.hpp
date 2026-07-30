#pragma once

#include "commands.hpp"

class cli_parser
{
public:
    Command parse_cmdline(int argc, const char* argv[]);
};