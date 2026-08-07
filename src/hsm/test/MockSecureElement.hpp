#pragma once

#include "ISecureElement.hpp"

class MockSecureElement : public ISecureElement
{
public:
    SecureElementStatus initResult = SecureElementStatus::OK;
    SecureElementStatus pingResult = SecureElementStatus::OK;
    SecureElementStatus eraseKeyResult = SecureElementStatus::OK;
    SecureElementStatus generateKeyResult = SecureElementStatus::OK;
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

    SecureElementStatus deinit() override
    {
        return deinitResult;
    }
};