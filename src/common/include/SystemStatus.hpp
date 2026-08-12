#pragma once

enum class SystemStatus
{
    OK,

    // HSM
    ERROR_INIT,
    ERROR_PING,
    ERROR_ERASE_KEY,
    ERROR_GENERATE_KEY,
    ERROR_READ_KEY,
    ERROR_DEINIT,
    ERROR_INVALID_CURVE,

    // SystemStatus
    SUCCESS,
    FAILED,

    // SystemStatus
    ERROR_DB,
    ERROR_DB_READ,
    ERROR_DB_WRITE,
};