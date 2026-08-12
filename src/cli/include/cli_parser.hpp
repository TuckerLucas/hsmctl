#pragma once

#include "cli_types.hpp"

class cli_parser
{
public:
    Command parseCommand(int argc, const char* argv[]);
};