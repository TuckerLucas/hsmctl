#pragma once

#include <string>

#include "CliTypes.hpp"

class CliParser
{
public:
    Command parseCommand(int argc, const char* argv[]);

private:
    // TODO: Consider refactoring this function somewhere else.
    static bool isValidHex(const std::string& str)
    {
        return str.find_first_not_of("0123456789abcdefABCDEF") == std::string::npos;
    }
};