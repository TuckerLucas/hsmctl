#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "AuditTypes.hpp"
#include "CliTypes.hpp"
#include "ISecureElement.hpp"

class CliDisplay
{
public:
    void helpMenu();
    void commandError(Command command);

    void statusResult(SystemStatus result);
    void logsResult(SystemStatus result, std::vector<AuditEntry> entries);
    void eraseKeyResult(SystemStatus result, uint8_t slot);
    void generateKeyResult(SystemStatus result, Command command);
    void readKeyResult(SystemStatus result, uint8_t slot, std::vector<uint8_t> pubKey);
    void listKeysResult(SystemStatus result, std::vector<std::vector<uint8_t>> pubKeys);

    // operation specific help menu functions
    void operationHelpMenu(Operation op);

    void status_help();
    void logs_help();
    void eraseKey_help();
    void generateKey_help();
    void readKey_help();
};