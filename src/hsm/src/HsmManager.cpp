#include "HsmManager.hpp"

HsmManager::HsmManager(ISecureElement& se, IAuditLogger& logger) : m_se(se), m_logger(logger)
{
}

SystemStatus HsmManager::status()
{
    SystemStatus result;
    Operation operation = Operation::STATUS;
    std::string options = "";

    result = m_se.init();

    if (result != SystemStatus::OK)
    {
        m_logger.log(operation, result, options);
        return result;
    }

    result = m_se.status();

    if (result != SystemStatus::OK)
    {
        m_logger.log(operation, result, options);
        return result;
    }

    result = m_se.deinit();

    m_logger.log(operation, result, options);

    return result;
}

SystemStatus HsmManager::eraseKey(uint8_t slot)
{
    SystemStatus result;
    Operation operation = Operation::ERASE_KEY;
    std::string options = "slot=" + std::to_string(slot);

    result = m_se.init();

    if (result != SystemStatus::OK)
    {
        m_logger.log(operation, result, options);
        return result;
    }

    result = m_se.eraseKey(slot);

    if (result != SystemStatus::OK)
    {
        m_logger.log(operation, result, options);
        return result;
    }

    result = m_se.deinit();

    m_logger.log(operation, result, options);

    return result;
}

SystemStatus HsmManager::generateKey(uint8_t slot, Curve curve)
{
    SystemStatus result;
    Operation operation = Operation::GENERATE_KEY;
    std::string curve_str = (curve == Curve::P256) ? "p256" : "ed25519";
    std::string options =
        "slot=" + std::to_string(slot) + (slot < 10 ? "  " : " ") + "curve=" + curve_str;

    result = m_se.init();

    if (result != SystemStatus::OK)
    {
        m_logger.log(operation, result, options);
        return result;
    }

    result = m_se.generateKey(slot, curve);

    if (result != SystemStatus::OK)
    {
        m_logger.log(operation, result, options);
        return result;
    }

    result = m_se.deinit();

    m_logger.log(operation, result, options);

    return result;
}

SystemStatus HsmManager::readKey(uint8_t slot, std::vector<uint8_t>& pubKey)
{
    SystemStatus result;

    result = m_se.init();

    if (result != SystemStatus::OK)
    {
        m_logger.log(Operation::READ_KEY, result, "slot=" + std::to_string(slot));
        return result;
    }

    result = m_se.readKey(slot, pubKey);

    if (result != SystemStatus::OK)
    {
        m_logger.log(Operation::READ_KEY, result, "slot=" + std::to_string(slot));
        return result;
    }

    result = m_se.deinit();

    m_logger.log(Operation::READ_KEY, result, "slot=" + std::to_string(slot));

    return result;
}