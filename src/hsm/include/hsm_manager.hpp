#pragma once

#include <cstdint>
#include <string>

#include "IAuditLogger.hpp"
#include "ISecureElement.hpp"

class hsm_manager
{
public:
    hsm_manager(ISecureElement& se, IAuditLogger& logger);

    SystemStatus status();
    SystemStatus eraseKey(uint8_t slot);
    SystemStatus generateKey(uint8_t slot, std::string curve);
    SystemStatus readKey(uint8_t slot, std::vector<uint8_t>& pubKey);

private:
    ISecureElement& m_se;
    IAuditLogger& m_logger;
};