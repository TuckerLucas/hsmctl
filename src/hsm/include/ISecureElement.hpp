#pragma once

#include <cstdint>

enum class SecureElementStatus
{
    OK,
    ERROR_INIT,
    ERROR_PING,
    ERROR_ERASE_KEY,
    ERROR_DEINIT,
};

class ISecureElement
{
public:
    virtual SecureElementStatus init() = 0;
    virtual SecureElementStatus ping() = 0;
    virtual SecureElementStatus eraseKey(uint8_t slot) = 0;
    virtual SecureElementStatus deinit() = 0;

    virtual ~ISecureElement() = default;
};