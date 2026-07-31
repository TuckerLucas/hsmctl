#include "cli_display.hpp"

void cli_display::help()
{
    std::cout << "hsmctl <COMMAND>\n";
    std::cout << "\n";
    std::cout << "HSM key management and audit logging\n";
    std::cout << "\n";
    std::cout << "Usage:\n";
    std::cout << "\n";
    std::cout << "\tGlobal flags:\n";
    std::cout << "\t\t--help       Show this help menu\n";
    std::cout << "\n";
    std::cout << "\tCommands:\n";
    std::cout << "\t\tstatus       Check hardware status\n";
    std::cout << "\n";
}

void cli_display::unknown(const std::string& input)
{
    std::cout << "Unknown command: '" << input << "'\n";
    std::cout << "Run 'hsmctl --help' for available commands\n";
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