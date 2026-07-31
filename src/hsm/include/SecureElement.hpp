#pragma once

#include "ISecureElement.hpp"

#include <memory>

class SecureElement : public ISecureElement
{
public:
    SecureElement();
    ~SecureElement();

    SecureElementStatus init() override;
    SecureElementStatus ping() override;
    SecureElementStatus deinit() override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};