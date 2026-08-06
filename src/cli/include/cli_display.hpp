#pragma once

#include <iostream>
#include <string>

#include "ISecureElement.hpp"
#include "cli_types.hpp"

class cli_display
{
private:
    std::string operationToString(Operation operation);

public:
    void help();
    void commandError(Command command);

    void status(SecureElementStatus result);
    void eraseKey(SecureElementStatus result, uint8_t slot);
    void generateKey(SecureElementStatus result, uint8_t slot);

    // operation specific help menu functions
    void status_help();
    void eraseKey_help();
    void generateKey_help();
};