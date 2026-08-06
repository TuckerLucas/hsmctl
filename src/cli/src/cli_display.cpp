#include "cli_display.hpp"

std::string cli_display::operationToString(Operation operation)
{
    switch (operation)
    {
        case Operation::ERASE_KEY:
            return "erase-key";
        case Operation::STATUS:
            return "status";
        default:
            return "unknown";
    }
}

void cli_display::help()
{
    std::cout << "hsmctl <operation> [options]\n";
    std::cout << "\n";
    std::cout << "HSM key management and audit logging\n";
    std::cout << "\n";
    std::cout << "Usage:\n";
    std::cout << "\n";
    std::cout << "\tGlobal flags:\n";
    std::cout << "\t\t--help       Show this help menu\n";
    std::cout << "\n";
    std::cout << "\tOperations:\n";
    std::cout << "\t\tstatus       Check hardware status\n";
    std::cout << "\t\terase-key    Erase a key\n";
    std::cout << "\n";
    std::cout << "Hint:\n";
    std::cout << "\n";
    std::cout << "\tRun 'hsmctl <operation> --help' for operation specific help\n";
}

void cli_display::commandError(Command command)
{
    switch (command.error)
    {
        case ParseError::MISSING_OPERATION:
        {
            std::cout << "No operation specified. Run 'hsmctl --help' for available operations\n";
            break;
        }
        case ParseError::INVALID_OPERATION:
        {
            std::cout
                << "Invalid operation specified. Run 'hsmctl --help' for available operations\n";
            break;
        }
        case ParseError::MISSING_OPTION:
        {
            std::cout << operationToString(command.operation) << ": needs an option\n";
            break;
        }
        case ParseError::INVALID_OPTION:
        {
            std::cout << operationToString(command.operation) << ": invalid option\n";
            break;
        }
        case ParseError::MISSING_VALUE:
        {
            std::cout << operationToString(command.operation) << " --slot: needs a value\n";
            break;
        }
        case ParseError::INVALID_VALUE:
        {
            std::cout << operationToString(command.operation)
                      << " --slot: supported slots are [0, 31]\n";
            break;
        }
    }
}

void cli_display::status(SecureElementStatus result)
{
    if (result == SecureElementStatus::OK)
    {
        std::cout << "Hardware detected!\n";
    }
    else
    {
        std::cout << "Could not detect hardware: ";

        if (result == SecureElementStatus::ERROR_INIT)
            std::cout << "initialisation error\n";
        else if (result == SecureElementStatus::ERROR_PING)
            std::cout << "ping failed\n";
        else if (result == SecureElementStatus::ERROR_DEINIT)
            std::cout << "deinitialisation error\n";
    }
}

void cli_display::eraseKey(SecureElementStatus result, uint8_t slot)
{
    if (result == SecureElementStatus::OK)
    {
        std::cout << "Key erased from slot " << static_cast<int>(slot) << "!\n";
    }
    else
    {
        std::cout << "Operation failed: ";

        if (result == SecureElementStatus::ERROR_INIT)
            std::cout << "initialisation error\n";
        else if (result == SecureElementStatus::ERROR_ERASE_KEY)
            std::cout << "erase failed\n";
        else if (result == SecureElementStatus::ERROR_DEINIT)
            std::cout << "deinitialisation error\n";
    }
}

void cli_display::status_help()
{
    std::cout << "Usage:\n";
    std::cout << "\thsmctl status\n";
    std::cout << "\n";
    std::cout << "Description:\n";
    std::cout << "\tChecks if the HSM is connected and responding\n";
    std::cout << "\n";
    std::cout << "Examples:\n";
    std::cout << "\thsmctl status\n";
}

void cli_display::eraseKey_help()
{
    std::cout << "Usage:\n";
    std::cout << "\thsmctl erase-key --slot <slot>\n";
    std::cout << "\n";
    std::cout << "Description:\n";
    std::cout << "\tErases the key stored in the specified slot on the HSM\n";
    std::cout << "\n";
    std::cout << "Options:\n";
    std::cout << "\t--slot <0-31>\tSlot number to erase\n";
    std::cout << "\n";
    std::cout << "Examples:\n";
    std::cout << "\thsmctl erase-key --slot 0\n";
    std::cout << "\thsmctl erase-key --slot 28\n";
}