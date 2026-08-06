#pragma once

#include <cstdint>

#include "ISecureElement.hpp"

class hsm_manager
{
public:
    hsm_manager(ISecureElement& se);

    SecureElementStatus status();
    SecureElementStatus eraseKey(uint8_t slot);
    SecureElementStatus generateKey(uint8_t slot);

private:
    ISecureElement& m_se;
};