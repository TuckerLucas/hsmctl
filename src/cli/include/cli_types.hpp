#pragma once

#include <string>
#include <unordered_map>

#define MIN_SLOT 0
#define MAX_SLOT 31

enum class Operation
{
    // Global flags
    HELP,

    // Operations
    STATUS,
    ERASE_KEY,
    GENERATE_KEY,

    NONE,
};

enum class ParseError
{
    MISSING_OPERATION,
    INVALID_OPERATION,
    MISSING_OPTION,
    INVALID_OPTION,
    MISSING_VALUE,
    INVALID_VALUE,

    NONE,
};

struct Command
{
    Operation operation = Operation::NONE;
    std::unordered_map<std::string, std::string> options;
    bool help = false;
    ParseError error = ParseError::NONE;
};