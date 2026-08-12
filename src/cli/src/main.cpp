#include <sys/stat.h>

#include "SecureElement.hpp"
#include "audit_logger.hpp"
#include "cli_display.hpp"
#include "cli_parser.hpp"
#include "hsm_manager.hpp"

std::string getDbPath();

// TODO: Rename test names, classes and files in consistent manner

int main(int argc, const char* argv[])
{
    cli_display display;
    cli_parser parser;
    Command command = parser.parseCommand(argc, argv);

    if (command.error != ParseError::NONE)
    {
        display.commandError(command);
        return 1;
    }

    if (command.requires_help)
    {
        display.operationHelpMenu(command.operation);
        return 0;
    }

    SecureElement se;
    audit_logger logger(getDbPath());
    hsm_manager hsm(se, logger);

    SystemStatus result;

    switch (command.operation)
    {
        case Operation::HELP:
        {
            display.helpMenu();
            return 0;
        }
        case Operation::STATUS:
        {
            result = hsm.status();

            display.statusResult(result);

            break;
        }
        case Operation::LOGS:
        {
            std::vector<AuditEntry> entries;

            result = logger.fetch(entries);

            display.logsResult(result, entries);

            break;
        }
        case Operation::ERASE_KEY:
        {
            uint8_t slot = std::stoi(command.options["slot"]);

            result = hsm.eraseKey(slot);

            display.eraseKeyResult(result, slot);

            break;
        }
        case Operation::GENERATE_KEY:
        {
            uint8_t slot = std::stoi(command.options["slot"]);
            std::string curve = command.options["curve"];

            result = hsm.generateKey(slot, curve);

            display.generateKeyResult(result, command);

            break;
        }
        case Operation::READ_KEY:
        {
            uint8_t slot = std::stoi(command.options["slot"]);
            std::vector<uint8_t> pubKey;

            result = hsm.readKey(slot, pubKey);

            display.readKeyResult(result, slot, pubKey);

            break;
        }
        case Operation::NONE:
        default:
            display.commandError(command);
            return 1;
    }

    return (result == SystemStatus::OK) ? 0 : 1;
}

std::string getDbPath()
{
    const char* home = getenv("HOME");
    std::string dir = std::string(home) + "/.hsmctl";
    mkdir(dir.c_str(), 0700);
    return dir + "/audit.db";
}