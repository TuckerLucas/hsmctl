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
    void helpMenu();
    void commandError(Command command);

    void statusResult(SystemStatus result);
    void logsResult(SystemStatus result, std::vector<AuditEntry> entries);
    void eraseKeyResult(SystemStatus result, uint8_t slot);
    void generateKeyResult(SystemStatus result, Command command);
    void readKeyResult(SystemStatus result, uint8_t slot, std::vector<uint8_t> pubKey);

    // operation specific help menu functions
    void operationHelpMenu(Operation op);

    void status_help();
    void logs_help();
    void eraseKey_help();
    void generateKey_help();
    void readKey_help();
};