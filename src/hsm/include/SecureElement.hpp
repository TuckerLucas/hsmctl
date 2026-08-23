#pragma once

#include <memory>

#include "ISecureElement.hpp"

class SecureElement : public ISecureElement
{
public:
    SecureElement();
    ~SecureElement();

    SystemStatus init() override;
    SystemStatus status() override;
    SystemStatus eraseKey(uint8_t slot) override;
    SystemStatus generateKey(uint8_t slot, Curve curve) override;
    SystemStatus readKey(uint8_t slot, std::vector<uint8_t>& pubKey) override;
    SystemStatus listKeys(std::vector<std::vector<uint8_t>>& pubKeys) override;
    SystemStatus sign(uint8_t slot, Curve curve, std::vector<uint8_t> payload,
                      std::vector<uint8_t>& signature) override;
    SystemStatus deinit() override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;

    SystemStatus signEd25519(uint8_t slot, std::vector<uint8_t> payload,
                             std::vector<uint8_t>& signature);

    SystemStatus signP256(uint8_t slot, std::vector<uint8_t> payload,
                          std::vector<uint8_t>& signature);
};