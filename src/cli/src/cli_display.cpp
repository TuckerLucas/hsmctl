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
    std::cout << "hsmctl <OPERATION> [OPTIONS]\n";
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
}

void cli_display::unknown(const std::string& input)
{
    std::cout << "Unknown operation: '" << input << "'\n";
    std::cout << "Run 'hsmctl --help' for available operations\n";
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
        std::cout << "Key erased from slot " << slot << "!\n";
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