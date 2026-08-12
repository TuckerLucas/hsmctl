#include "hsm_manager.hpp"

hsm_manager::hsm_manager(ISecureElement& se, IAuditLogger& logger) : m_se(se), m_logger(logger)
{
}

SystemStatus hsm_manager::status()
{
    SystemStatus result;

    result = m_se.init();

    if (result != SystemStatus::OK)
    {
        m_logger.log(Operation::STATUS, SystemStatus::FAILED, "");
        return result;
    }

    result = m_se.ping();

    if (result != SystemStatus::OK)
    {
        m_logger.log(Operation::STATUS, SystemStatus::FAILED, "");
        return result;
    }

    result = m_se.deinit();

    if (result != SystemStatus::OK)
    {
        m_logger.log(Operation::STATUS, SystemStatus::FAILED, "");
        return result;
    }

    m_logger.log(Operation::STATUS, SystemStatus::SUCCESS, "");

    return result;
}

SystemStatus hsm_manager::eraseKey(uint8_t slot)
{
    SystemStatus result;

    result = m_se.init();

    if (result != SystemStatus::OK)
    {
        m_logger.log(Operation::ERASE_KEY, SystemStatus::FAILED, "slot=" + std::to_string(slot));
        return result;
    }

    result = m_se.eraseKey(slot);

    if (result != SystemStatus::OK)
    {
        m_logger.log(Operation::ERASE_KEY, SystemStatus::FAILED, "slot=" + std::to_string(slot));
        return result;
    }

    result = m_se.deinit();

    if (result != SystemStatus::OK)
    {
        m_logger.log(Operation::ERASE_KEY, SystemStatus::FAILED, "slot=" + std::to_string(slot));
        return result;
    }

    m_logger.log(Operation::ERASE_KEY, SystemStatus::SUCCESS, "slot=" + std::to_string(slot));

    return result;
}

SystemStatus hsm_manager::generateKey(uint8_t slot, std::string curve_str)
{
    Curve curve;

    // TODO: check if this logic should be here or in parser
    if (curve_str == "ed25519")
    {
        curve = Curve::Ed25519;
    }
    else if (curve_str == "p256")
    {
        curve = Curve::P256;
    }
    else
    {
        return SystemStatus::ERROR_INVALID_CURVE;
    }

    SystemStatus result;

    result = m_se.init();

    if (result != SystemStatus::OK)
    {
        m_logger.log(
            Operation::GENERATE_KEY, SystemStatus::FAILED,
            "slot=" + std::to_string(slot) + (slot < 10 ? "  " : " ") + "curve=" + curve_str);
        return result;
    }

    result = m_se.generateKey(slot, curve);

    if (result != SystemStatus::OK)
    {
        m_logger.log(
            Operation::GENERATE_KEY, SystemStatus::FAILED,
            "slot=" + std::to_string(slot) + (slot < 10 ? "  " : " ") + "curve=" + curve_str);
        return result;
    }

    result = m_se.deinit();

    if (result != SystemStatus::OK)
    {
        m_logger.log(
            Operation::GENERATE_KEY, SystemStatus::FAILED,
            "slot=" + std::to_string(slot) + (slot < 10 ? "  " : " ") + "curve=" + curve_str);
        return result;
    }

    m_logger.log(Operation::GENERATE_KEY, SystemStatus::SUCCESS,
                 "slot=" + std::to_string(slot) + (slot < 10 ? "  " : " ") + "curve=" + curve_str);

    return result;
}

SystemStatus hsm_manager::readKey(uint8_t slot, std::vector<uint8_t>& pubKey)
{
    SystemStatus result;

    result = m_se.init();

    if (result != SystemStatus::OK)
    {
        m_logger.log(Operation::READ_KEY, SystemStatus::FAILED, "slot=" + std::to_string(slot));
        return SystemStatus::ERROR_INIT;
    }

    result = m_se.readKey(slot, pubKey);

    if (result != SystemStatus::OK)
    {
        m_logger.log(Operation::READ_KEY, SystemStatus::FAILED, "slot=" + std::to_string(slot));
        return SystemStatus::ERROR_READ_KEY;
    }

    result = m_se.deinit();

    if (result != SystemStatus::OK)
    {
        m_logger.log(Operation::READ_KEY, SystemStatus::FAILED, "slot=" + std::to_string(slot));
        return SystemStatus::ERROR_DEINIT;
    }

    m_logger.log(Operation::READ_KEY, SystemStatus::SUCCESS, "slot=" + std::to_string(slot));

    return result;
}