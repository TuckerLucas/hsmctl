#include "hsm_manager.hpp"

hsm_manager::hsm_manager(ISecureElement& se) : m_se(se)
{
}

SecureElementStatus hsm_manager::status()
{
    SecureElementStatus result;

    result = m_se.init();

    if (result != SecureElementStatus::OK)
    {
        return result;
    }

    result = m_se.ping();

    if (result != SecureElementStatus::OK)
    {
        return result;
    }

    result = m_se.deinit();

    return result;
}

SecureElementStatus hsm_manager::eraseKey(uint8_t slot)
{
    SecureElementStatus result;

    result = m_se.init();

    if (result != SecureElementStatus::OK)
    {
        return result;
    }

    result = m_se.eraseKey(slot);

    if (result != SecureElementStatus::OK)
    {
        return result;
    }

    result = m_se.deinit();

    if (result != SecureElementStatus::OK)
    {
        return result;
    }

    return SecureElementStatus::OK;
}