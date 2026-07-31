#pragma once

enum class Command
{
    // Global flags
    HELP,

    // Commands
    INJECT_KEY,
    UPDATE_KEY,
    GET_KEY,
    ERASE_KEY,

    UNKNOWN,
};