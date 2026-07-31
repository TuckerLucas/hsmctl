#pragma once

enum class Command
{
    // Global flags
    HELP,

    // Commands
    STATUS,

    UNKNOWN,
    NONE,
};

class cli_parser
{
public:
    Command parse_cmdline(int argc, const char* argv[]);
};