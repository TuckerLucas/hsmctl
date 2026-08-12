#pragma once

#include <cstdint>
#include <vector>

#include "HsmTypes.hpp"
#include "SystemStatus.hpp"

class ISecureElement
{
public:
    virtual SystemStatus init() = 0;
    virtual SystemStatus status() = 0;
    virtual SystemStatus eraseKey(uint8_t slot) = 0;
    virtual SystemStatus generateKey(uint8_t slot, Curve curve) = 0;
    virtual SystemStatus readKey(uint8_t slot, std::vector<uint8_t>& pubKey) = 0;
    virtual SystemStatus deinit() = 0;

    virtual ~ISecureElement() = default;
};