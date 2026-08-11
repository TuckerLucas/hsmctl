#pragma once

#include "ISecureElement.hpp"

const std::vector<uint8_t> mock_public_key = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};

class MockSecureElement : public ISecureElement
{
public:
    SecureElementStatus initResult = SecureElementStatus::OK;
    SecureElementStatus pingResult = SecureElementStatus::OK;
    SecureElementStatus eraseKeyResult = SecureElementStatus::OK;
    SecureElementStatus generateKeyResult = SecureElementStatus::OK;
    SecureElementStatus readKeyResult = SecureElementStatus::OK;
    SecureElementStatus deinitResult = SecureElementStatus::OK;

    SecureElementStatus init() override
    {
        return initResult;
    }

    SecureElementStatus ping() override
    {
        return pingResult;
    }

    SecureElementStatus eraseKey(uint8_t) override
    {
        return eraseKeyResult;
    }

    SecureElementStatus generateKey(uint8_t, Curve) override
    {
        return generateKeyResult;
    }

    SecureElementStatus readKey(uint8_t, std::vector<uint8_t>& pubKey) override
    {
        pubKey = mock_public_key;

        return readKeyResult;
    }

    SecureElementStatus deinit() override
    {
        return deinitResult;
    }
};