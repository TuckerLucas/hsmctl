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
    void unknown(const std::string& input);
    void commandError(Command command);

    void status(SecureElementStatus result);
    void eraseKey(SecureElementStatus result, uint8_t slot);
};