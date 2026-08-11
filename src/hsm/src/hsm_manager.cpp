#include "hsm_manager.hpp"

hsm_manager::hsm_manager(ISecureElement& se, IAuditLogger& logger) : m_se(se), m_logger(logger)
{
}

SecureElementStatus hsm_manager::status()
{
    SecureElementStatus result;

    result = m_se.init();

    if (result != SecureElementStatus::OK)
    {
        m_logger.log(Operation::STATUS, AuditResult::FAILED, "");
        return result;
    }

    result = m_se.ping();

    if (result != SecureElementStatus::OK)
    {
        m_logger.log(Operation::STATUS, AuditResult::FAILED, "");
        return result;
    }

    result = m_se.deinit();

    if (result != SecureElementStatus::OK)
    {
        m_logger.log(Operation::STATUS, AuditResult::FAILED, "");
        return result;
    }

    m_logger.log(Operation::STATUS, AuditResult::SUCCESS, "");

    return result;
}

SecureElementStatus hsm_manager::eraseKey(uint8_t slot)
{
    SecureElementStatus result;

    result = m_se.init();

    if (result != SecureElementStatus::OK)
    {
        m_logger.log(Operation::ERASE_KEY, AuditResult::FAILED, "slot=" + std::to_string(slot));
        return result;
    }

    result = m_se.eraseKey(slot);

    if (result != SecureElementStatus::OK)
    {
        m_logger.log(Operation::ERASE_KEY, AuditResult::FAILED, "slot=" + std::to_string(slot));
        return result;
    }

    result = m_se.deinit();

    if (result != SecureElementStatus::OK)
    {
        m_logger.log(Operation::ERASE_KEY, AuditResult::FAILED, "slot=" + std::to_string(slot));
        return result;
    }

    m_logger.log(Operation::ERASE_KEY, AuditResult::SUCCESS, "slot=" + std::to_string(slot));

    return result;
}

SecureElementStatus hsm_manager::generateKey(uint8_t slot, std::string curve_str)
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
        return SecureElementStatus::ERROR_INVALID_CURVE;
    }

    SecureElementStatus result;

    result = m_se.init();

    if (result != SecureElementStatus::OK)
    {
        m_logger.log(
            Operation::GENERATE_KEY, AuditResult::FAILED,
            "slot=" + std::to_string(slot) + (slot < 10 ? "  " : " ") + "curve=" + curve_str);
        return result;
    }

    result = m_se.generateKey(slot, curve);

    if (result != SecureElementStatus::OK)
    {
        m_logger.log(
            Operation::GENERATE_KEY, AuditResult::FAILED,
            "slot=" + std::to_string(slot) + (slot < 10 ? "  " : " ") + "curve=" + curve_str);
        return result;
    }

    result = m_se.deinit();

    if (result != SecureElementStatus::OK)
    {
        m_logger.log(
            Operation::GENERATE_KEY, AuditResult::FAILED,
            "slot=" + std::to_string(slot) + (slot < 10 ? "  " : " ") + "curve=" + curve_str);
        return result;
    }

    m_logger.log(Operation::GENERATE_KEY, AuditResult::SUCCESS,
                 "slot=" + std::to_string(slot) + (slot < 10 ? "  " : " ") + "curve=" + curve_str);

    return result;
}

SecureElementStatus hsm_manager::readKey(uint8_t slot, std::vector<uint8_t>& pubKey)
{
    SecureElementStatus result;

    result = m_se.init();

    if (result != SecureElementStatus::OK)
    {
        m_logger.log(Operation::READ_KEY, AuditResult::FAILED, "slot=" + std::to_string(slot));
        return SecureElementStatus::ERROR_INIT;
    }

    result = m_se.readKey(slot, pubKey);

    if (result != SecureElementStatus::OK)
    {
        m_logger.log(Operation::READ_KEY, AuditResult::FAILED, "slot=" + std::to_string(slot));
        return SecureElementStatus::ERROR_READ_KEY;
    }

    result = m_se.deinit();

    if (result != SecureElementStatus::OK)
    {
        m_logger.log(Operation::READ_KEY, AuditResult::FAILED, "slot=" + std::to_string(slot));
        return SecureElementStatus::ERROR_DEINIT;
    }

    m_logger.log(Operation::READ_KEY, AuditResult::SUCCESS, "slot=" + std::to_string(slot));

    return result;
}