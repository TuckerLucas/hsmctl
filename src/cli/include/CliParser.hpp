#pragma once

#include "CliTypes.hpp"

class CliParser
{
public:
    Command parseCommand(int argc, const char* argv[]);
};