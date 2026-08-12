#pragma once

#include <cstdint>
#include <string>

#include "IAuditLogger.hpp"
#include "ISecureElement.hpp"

class HsmManager
{
public:
    HsmManager(ISecureElement& se, IAuditLogger& logger);

    SystemStatus status();
    SystemStatus eraseKey(uint8_t slot);
    SystemStatus generateKey(uint8_t slot, std::string curve);
    SystemStatus readKey(uint8_t slot, std::vector<uint8_t>& pubKey);

private:
    ISecureElement& m_se;
    IAuditLogger& m_logger;
};