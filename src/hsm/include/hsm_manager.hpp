#pragma once

#include <cstdint>
#include <string>

#include "IAuditLogger.hpp"
#include "ISecureElement.hpp"

class hsm_manager
{
public:
    hsm_manager(ISecureElement& se, IAuditLogger& logger);

    SecureElementStatus status();
    SecureElementStatus eraseKey(uint8_t slot);
    SecureElementStatus generateKey(uint8_t slot, std::string curve);

private:
    ISecureElement& m_se;
    IAuditLogger& m_logger;
};