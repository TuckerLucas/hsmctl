#pragma once

#include "ISecureElement.hpp"

#include <iostream>
#include <string>

class cli_display
{
public:
    void help();
    void unknown(const std::string& input);
    void status(SecureElementStatus result);
};