#pragma once

#include <memory>

#include "ISecureElement.hpp"

class SecureElement : public ISecureElement
{
public:
    SecureElement();
    ~SecureElement();

    SystemStatus init() override;
    SystemStatus ping() override;
    SystemStatus eraseKey(uint8_t slot) override;
    SystemStatus generateKey(uint8_t slot, Curve curve) override;
    SystemStatus readKey(uint8_t slot, std::vector<uint8_t>& pubKey) override;
    SystemStatus deinit() override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};