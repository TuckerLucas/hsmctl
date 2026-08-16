#include "CliDisplay.hpp"

#include <cassert>
#include <iomanip>

void CliDisplay::helpMenu()
{
    std::cout << "hsmctl <operation> [options]\n";
    std::cout << "\n";
    std::cout << "HSM key management and audit logging.\n";
    std::cout << "\n";
    std::cout << "Usage:\n";
    std::cout << "\n";
    std::cout << "        Global flags:\n";
    std::cout << "                     --help         Show this help menu.\n";
    std::cout << "\n";
    std::cout << "        Operations:\n";
    std::cout << "                     status         Scan hardware status.\n";
    std::cout << "                     logs           Display HSM audit logs.\n";
    std::cout << "                     erase-key      Erase a key.\n";
    std::cout << "                     generate-key   Generate an ECC key pair.\n";
    std::cout << "                     read-key       Read a public key.\n";
    std::cout << "                     list-keys      Read all existing public keys.\n";
    std::cout << "\n";
    std::cout << "Hint:\n";
    std::cout << "\n";
    std::cout << "        Run 'hsmctl <operation> --help' for operation specific options.\n";
}

void CliDisplay::commandError(Command command)
{
    switch (command.error)
    {
        case ParseError::MISSING_OPERATION:
        {
            std::cout << "No operation specified. Run 'hsmctl --help' for available operations.\n";
            break;
        }
        case ParseError::INVALID_OPERATION:
        {
            std::cout
                << "Invalid operation specified. Run 'hsmctl --help' for available operations.\n";
            break;
        }
        case ParseError::MISSING_OPTION:
        {
            std::cout << operationToString(command.operation) << ": missing an option.\n";
            break;
        }
        case ParseError::INVALID_OPTION:
        {
            std::cout << operationToString(command.operation) << ": invalid option.\n";
            break;
        }
        case ParseError::MISSING_VALUE:
        {
            std::cout << operationToString(command.operation) << ": missing option value.\n";
            break;
        }
        case ParseError::INVALID_VALUE:
        {
            std::cout << operationToString(command.operation) << ": invalid value.\n";
            break;
        }
    }
}

void CliDisplay::statusResult(SystemStatus result)
{
    if (result == SystemStatus::OK)
    {
        std::cout << "Hardware detected!\n";
    }
    else
    {
        std::cout << "Could not detect hardware: ";

        if (result == SystemStatus::HSM_ERROR_INIT)
            std::cout << "initialisation error. Check that the security module is connected.\n";
        else if (result == SystemStatus::HSM_ERROR_STATUS)
            std::cout << "ping failed. Check that the security module is connected.\n";
        else if (result == SystemStatus::HSM_ERROR_DEINIT)
            std::cout << "deinitialisation error.\n";
    }
}

void CliDisplay::logsResult(SystemStatus result, std::vector<AuditEntry> entries)
{
    if (result == SystemStatus::OK)
    {
        std::cout << "\n";
        std::cout << "Audit Log\n";
        std::cout << std::string(75, '-') << "\n";
        std::cout << std::left << std::setw(22) << "Timestamp" << std::setw(15) << "Operation"
                  << std::setw(10) << "Result" << std::setw(20) << "Options"
                  << "\n";
        std::cout << std::string(75, '-') << "\n";

        for (const auto& entry : entries)
        {
            std::cout << std::left << std::setw(22) << entry.timestamp << std::setw(15)
                      << entry.operation << std::setw(10) << entry.auditResult << std::setw(20)
                      << entry.options << "\n";
        }

        std::cout << std::string(75, '-') << "\n";
        std::cout << entries.size() << " operations logged\n\n";
    }
    else
    {
        std::cout << "Failed to retrieve logs\n";
    }
}

void CliDisplay::eraseKeyResult(SystemStatus result, uint8_t slot)
{
    if (result == SystemStatus::OK)
    {
        std::cout << "Key erased from slot " << static_cast<int>(slot) << "!\n";
    }
    else
    {
        std::cout << "Operation failed: ";

        if (result == SystemStatus::HSM_ERROR_INIT)
            std::cout << "initialisation error. Check that the security module is connected.\n";
        else if (result == SystemStatus::HSM_ERROR_ERASE_KEY)
            std::cout << "could not erase key.\n";
        else if (result == SystemStatus::HSM_ERROR_DEINIT)
            std::cout << "deinitialisation error.\n";
    }
}

void CliDisplay::generateKeyResult(SystemStatus result, Command command)
{
    if (result == SystemStatus::OK)
    {
        std::cout << "Key generated in slot " << command.options["slot"] << " with "
                  << command.options["curve"] << " curve!\n";
    }
    else
    {
        std::cout << "Operation failed: ";

        if (result == SystemStatus::HSM_ERROR_INIT)
        {
            std::cout << "initialisation error. Check that the security module is connected.\n";
        }
        else if (result == SystemStatus::HSM_ERROR_GENERATE_KEY_SLOT_OCCUPIED)
        {
            std::cout << "slot " << command.options["slot"] << " already contains a key.\n";
            std::cout << "Run 'hsmctl erase-key --slot " << command.options["slot"]
                      << "' to erase it first.\n";
        }
        else if (result == SystemStatus::HSM_ERROR_GENERATE_KEY_HW_ERROR)
        {
            std::cout << "hardware error generating key in slot " << command.options["slot"]
                      << ".\n";
        }
        else if (result == SystemStatus::HSM_ERROR_DEINIT)
        {
            std::cout << "deinitialisation error.\n";
        }
    }
}

void CliDisplay::readKeyResult(SystemStatus result, uint8_t slot, std::vector<uint8_t> pubKey)
{
    if (result == SystemStatus::OK)
    {
        std::string curve_name = (pubKey.size() == 32) ? "Ed25519" : "P-256";

        std::cout << "\n";
        std::cout << "Public key from slot " << static_cast<int>(slot) << " (" << curve_name
                  << "):\n";
        std::cout << std::string(60, '-') << "\n";

        for (size_t i = 0; i < pubKey.size(); i++)
        {
            std::cout << std::hex << std::setw(2) << std::setfill('0')
                      << static_cast<int>(pubKey[i]);

            if ((i + 1) % 16 == 0)
                std::cout << "\n";
            else
                std::cout << " ";
        }

        std::cout << std::string(60, '-') << "\n";
        std::cout << "Key length: " << std::dec << pubKey.size() << " bytes\n\n";
    }
    else
    {
        std::cout << "Operation failed: ";

        if (result == SystemStatus::HSM_ERROR_INIT)
        {
            std::cout << "initialisation error. Check that the security module is connected.\n";
        }
        else if (result == SystemStatus::HSM_ERROR_READ_KEY_EMPTY_SLOT)
        {
            std::cout << "slot " << static_cast<int>(slot) << " is empty.";
            std::cout << "\n";
            std::cout << "Run 'hsmctl generate-key --slot " << static_cast<int>(slot)
                      << "' to generate a key first.\n";
        }
        else if (result == SystemStatus::HSM_ERROR_READ_KEY_HW_ERROR)
        {
            std::cout << "hardware error reading key from slot " << static_cast<int>(slot) << ".\n";
        }
        else if (result == SystemStatus::HSM_ERROR_DEINIT)
        {
            std::cout << "deinitialisation error.\n";
        }
    }
}

void CliDisplay::listKeysResult(SystemStatus result, std::vector<std::vector<uint8_t>> pubKeys)
{
    if (result != SystemStatus::OK)
    {
        std::cout << "Error reading keys\n";
        return;
    }

    int occupied = 0;
    for (size_t i = 0; i < pubKeys.size(); i++)
    {
        if (!pubKeys[i].empty())
            occupied++;
    }

    if (occupied == 0)
    {
        std::cout << "\nNo keys found.\n\n";
        return;
    }

    std::cout << "\n";
    std::cout << std::string(60, '-') << "\n";
    std::cout << std::left << std::setw(8) << "Slot" << std::setw(10) << "Curve"
              << "Public Key\n";
    std::cout << std::string(60, '-') << "\n";

    for (size_t i = 0; i < pubKeys.size(); i++)
    {
        if (pubKeys[i].empty())
            continue;

        std::string curve = (pubKeys[i].size() == 32) ? "Ed25519" : "P-256";

        std::cout << std::left << std::setw(8) << i << std::setw(10) << curve;

        for (size_t j = 0; j < pubKeys[i].size(); j++)
        {
            std::cout << std::hex << std::setw(2) << std::setfill('0')
                      << static_cast<int>(pubKeys[i][j]);
        }

        std::cout << std::dec << std::setfill(' ') << "\n";
    }

    std::cout << std::string(60, '-') << "\n";
    std::cout << occupied << " key(s) found\n\n";
}

void CliDisplay::status_help()
{
    std::cout << "Usage:\n";
    std::cout << "        hsmctl status\n";
    std::cout << "\n";
    std::cout << "Description:\n";
    std::cout << "        Checks if the HSM is connected and responding.\n";
    std::cout << "\n";
    std::cout << "Examples:\n";
    std::cout << "        hsmctl status\n";
}

void CliDisplay::logs_help()
{
    std::cout << "Usage:\n";
    std::cout << "        hsmctl logs\n";
    std::cout << "\n";
    std::cout << "Description:\n";
    std::cout << "        Display audit log of all operations performed on the HSM.\n";
    std::cout << "\n";
    std::cout << "Examples:\n";
    std::cout << "        hsmctl logs\n";
}

void CliDisplay::eraseKey_help()
{
    std::cout << "Usage:\n";
    std::cout << "        hsmctl erase-key --slot <slot>\n";
    std::cout << "\n";
    std::cout << "Description:\n";
    std::cout << "        Erases the key stored in the specified slot on the HSM.\n";
    std::cout << "\n";
    std::cout << "Options:\n";
    std::cout << "        --slot <" << MIN_SLOT << ", " << MAX_SLOT
              << ">    Slot number to erase.\n";
    std::cout << "\n";
    std::cout << "Examples:\n";
    std::cout << "        hsmctl erase-key --slot 0\n";
    std::cout << "        hsmctl erase-key --slot 28\n";
}

void CliDisplay::generateKey_help()
{
    std::cout << "Usage:\n";
    std::cout << "        hsmctl generate-key --slot <slot>\n";
    std::cout << "\n";
    std::cout << "Description:\n";
    std::cout << "        Generates an ECC key pair in the specified slot on the HSM.\n";
    std::cout << "\n";
    std::cout << "Options:\n";
    std::cout << "        --slot  <" << MIN_SLOT << ", " << MAX_SLOT
              << ">   Slot number to erase.\n";
    std::cout << "        --curve <curve>   Curve to use for key generation\n";
    std::cout << "                          ed25519   Edwards curve (default)\n";
    std::cout << "                          p256      NIST P-256 curve\n";
    std::cout << "\n";
    std::cout << "Examples:\n";
    std::cout << "        hsmctl generate-key --slot 3\n";
    std::cout << "        hsmctl generate-key --slot 3 --curve ed25519\n";
    std::cout << "        hsmctl generate-key --slot 3 --curve p256\n";
    std::cout << "        hsmctl generate-key --curve p256 --slot 3\n";
}

void CliDisplay::readKey_help()
{
    std::cout << "Usage:\n";
    std::cout << "        hsmctl read-key --slot <slot>\n";
    std::cout << "\n";
    std::cout << "Description:\n";
    std::cout << "        Reads the public key stored in the specified slot on the HSM.\n";
    std::cout << "\n";
    std::cout << "Options:\n";
    std::cout << "        --slot <" << MIN_SLOT << ", " << MAX_SLOT
              << ">    Slot number to erase.\n";
    std::cout << "\n";
    std::cout << "Examples:\n";
    std::cout << "        hsmctl read-key --slot 2\n";
    std::cout << "        hsmctl read-key --slot 31\n";
}

void CliDisplay::operationHelpMenu(Operation op)
{
    switch (op)
    {
        case Operation::STATUS:
            status_help();
            break;
        case Operation::LOGS:
            logs_help();
            break;
        case Operation::ERASE_KEY:
            eraseKey_help();
            break;
        case Operation::GENERATE_KEY:
            generateKey_help();
            break;
        case Operation::READ_KEY:
            readKey_help();
            break;
        default:
            assert(false && "Unhandled operation value in operationHelpMenu");
    }
}