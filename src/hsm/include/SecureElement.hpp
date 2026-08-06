#pragma once

#include <memory>

#include "ISecureElement.hpp"

class SecureElement : public ISecureElement
{
public:
    SecureElement();
    ~SecureElement();

    SecureElementStatus init() override;
    SecureElementStatus ping() override;
    SecureElementStatus eraseKey(uint8_t slot) override;
    SecureElementStatus generateKey(uint8_t slot) override;
    SecureElementStatus deinit() override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};