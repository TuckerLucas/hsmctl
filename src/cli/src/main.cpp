#include <sys/stat.h>

#include <fstream>

#include "AuditLogger.hpp"
#include "CliDisplay.hpp"
#include "CliParser.hpp"
#include "HsmManager.hpp"
#include "SecureElement.hpp"

std::string getDbPath();

int main(int argc, const char* argv[])
{
    CliDisplay display;
    CliParser parser;
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
    AuditLogger logger(getDbPath());
    HsmManager hsm(se, logger);

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
            Curve curve = (command.options["curve"] == "p256") ? Curve::P256 : Curve::Ed25519;

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
        case Operation::LIST_KEYS:
        {
            std::vector<std::vector<uint8_t>> pubKeys;

            result = hsm.listKeys(pubKeys);

            display.listKeysResult(result, command, pubKeys);

            break;
        }
        // TODO: Hash the payload contents before signing
        case Operation::SIGN:
        {
            uint8_t slot = std::stoi(command.options["slot"]);
            std::vector<uint8_t> payload;
            std::vector<uint8_t> signature;
            SignSource signSource;
            std::string filepath = "";

            if (command.options.count("data") > 0)
            {
                signSource = SignSource::DATA;
                std::string data = command.options["data"];
                payload = std::vector<uint8_t>(data.begin(), data.end());
            }
            else
            {
                signSource = SignSource::FILE;
                filepath = command.options["file"];
                std::ifstream file(filepath, std::ios::binary);

                if (!file.is_open())
                {
                    display.signResult(SystemStatus::ERROR_FILE_NOT_FOUND, {});
                    return 1;
                }

                payload = std::vector<uint8_t>(std::istreambuf_iterator<char>(file),
                                               std::istreambuf_iterator<char>());

                // Added for safety
                // Should already be handled by RAII
                file.close();
            }

            result = hsm.sign(slot, payload, signature, signSource, filepath);

            display.signResult(result, signature);

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