#pragma once

#include "ISecureElement.hpp"

class hsm_manager
{
public:
    hsm_manager(ISecureElement& se);

    SecureElementStatus status();

private:
    ISecureElement& m_se;
};