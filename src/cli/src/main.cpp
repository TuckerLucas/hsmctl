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

    if (argc < 2)
    {
        display.help();
        return 0;
    }

    cli_parser parser;
    Command command = parser.parse_cmdline(argc, argv);

    if (command.error != ParseError::NONE)
    {
        display.commandError(command);
        return 1;
    }

    // TODO: Refactor these two lines
    SecureElement se;
    audit_logger logger(getDbPath());
    hsm_manager hsm(se, logger);

    switch (command.operation)
    {
        case Operation::HELP:
        {
            display.help();
            return 0;
        }
        case Operation::STATUS:
        {
            if (command.requires_help)
            {
                display.status_help();
                return 0;
            }

            SecureElementStatus result = hsm.status();

            display.status(result);

            return (result == SecureElementStatus::OK) ? 0 : 1;
        }
        case Operation::LOGS:
        {
            if (command.requires_help)
            {
                display.logs_help();
                return 0;
            }

            std::vector<AuditEntry> entries;

            auto result = logger.fetch(entries);

            display.logs(result, entries);

            return (result == AuditStatus::OK) ? 0 : 1;
        }
        case Operation::ERASE_KEY:
        {
            if (command.requires_help)
            {
                display.eraseKey_help();
                return 0;
            }

            uint8_t slot = std::stoi(command.options["slot"]);
            SecureElementStatus result = hsm.eraseKey(slot);

            display.eraseKey(result, slot);

            return (result == SecureElementStatus::OK) ? 0 : 1;
        }
        case Operation::GENERATE_KEY:
        {
            if (command.requires_help)
            {
                display.generateKey_help();
                return 0;
            }

            uint8_t slot = std::stoi(command.options["slot"]);
            std::string curve = command.options["curve"];
            SecureElementStatus result = hsm.generateKey(slot, curve);

            display.generateKey(result, command);

            return (result == SecureElementStatus::OK) ? 0 : 1;
        }
        case Operation::READ_KEY:
        {
            if (command.requires_help)
            {
                display.readKey_help();
                return 0;
            }

            uint8_t slot = std::stoi(command.options["slot"]);
            std::vector<uint8_t> pubKey;

            SecureElementStatus result = hsm.readKey(slot, pubKey);

            display.readKey(result, slot, pubKey);

            return (result == SecureElementStatus::OK) ? 0 : 1;
        }
        case Operation::NONE:
        default:
            display.commandError(command);
            return 1;
    }

    return 1;
}

std::string getDbPath()
{
    const char* home = getenv("HOME");
    std::string dir = std::string(home) + "/.hsmctl";
    mkdir(dir.c_str(), 0700);
    return dir + "/audit.db";
}