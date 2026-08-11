#pragma once

#include <cstdint>
#include <vector>

#include "hsm_types.hpp"

enum class SecureElementStatus
{
    OK,
    ERROR_INIT,
    ERROR_PING,
    ERROR_ERASE_KEY,
    ERROR_GENERATE_KEY,
    ERROR_READ_KEY,
    ERROR_DEINIT,
    ERROR_INVALID_CURVE
};

class ISecureElement
{
public:
    virtual SecureElementStatus init() = 0;
    virtual SecureElementStatus ping() = 0;
    virtual SecureElementStatus eraseKey(uint8_t slot) = 0;
    virtual SecureElementStatus generateKey(uint8_t slot, Curve curve) = 0;
    virtual SecureElementStatus readKey(uint8_t slot, std::vector<uint8_t>& pubKey) = 0;
    virtual SecureElementStatus deinit() = 0;

    virtual ~ISecureElement() = default;
};