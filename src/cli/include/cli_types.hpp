#pragma once

#include <string>
#include <unordered_map>

enum class Operation
{
    // Global flags
    HELP,

    // Operations
    STATUS,
    ERASE_KEY,

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
    ParseError error = ParseError::NONE;
};