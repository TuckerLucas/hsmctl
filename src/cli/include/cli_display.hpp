#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "ISecureElement.hpp"
#include "audit_types.hpp"
#include "cli_types.hpp"

class cli_display
{
private:
    std::string operationToString(Operation operation);

public:
    void help();
    void commandError(Command command);

    void status(SecureElementStatus result);
    void logs(AuditStatus result, std::vector<AuditEntry> entries);
    void eraseKey(SecureElementStatus result, uint8_t slot);
    void generateKey(SecureElementStatus result, Command command);
    void readKey(SecureElementStatus result, uint8_t slot, std::vector<uint8_t> pubKey);

    void operationHelpMenu(Operation op);

    // operation specific help menu functions
    void status_help();
    void logs_help();
    void eraseKey_help();
    void generateKey_help();
    void readKey_help();
};