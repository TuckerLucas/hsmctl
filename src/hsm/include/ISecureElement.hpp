#pragma once

enum class SecureElementStatus
{
    OK,
    ERROR_INIT,
    ERROR_PING,
    ERROR_DEINIT,
};

class ISecureElement
{
public:
    virtual SecureElementStatus init() = 0;
    virtual SecureElementStatus ping() = 0;
    virtual SecureElementStatus deinit() = 0;

    virtual ~ISecureElement() = default;
};