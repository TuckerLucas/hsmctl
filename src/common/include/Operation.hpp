#pragma once

#include <cassert>
#include <string>
#include <unordered_map>

enum class Operation
{
    // Global flags
    HELP,

    // Operations
    STATUS,
    LOGS,
    ERASE_KEY,
    GENERATE_KEY,
    READ_KEY,

    NONE,
};

inline std::string operationToString(Operation operation)
{
    static const std::unordered_map<Operation, std::string> op_map = {
        {Operation::STATUS, "status"},       {Operation::LOGS, "logs"},
        {Operation::ERASE_KEY, "erase-key"}, {Operation::GENERATE_KEY, "generate-key"},
        {Operation::READ_KEY, "read-key"},
    };

    auto it = op_map.find(operation);
    assert(it != op_map.end() && "Unhandled operation value");
    return (it != op_map.end()) ? it->second : "UNKNOWN";
}