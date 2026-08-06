#include "SecureElement.hpp"
#include "cli_display.hpp"
#include "cli_parser.hpp"
#include "hsm_manager.hpp"

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

    switch (command.operation)
    {
        case Operation::HELP:
        {
            display.help();

            break;
        }
        case Operation::STATUS:
        {
            SecureElement se;
            hsm_manager hsm(se);

            SecureElementStatus result = hsm.status();

            display.status(result);

            break;
        }
        case Operation::ERASE_KEY:
        {
            SecureElement se;
            hsm_manager hsm(se);

            uint8_t slot = std::stoi(command.options["slot"]);
            SecureElementStatus result = hsm.eraseKey(slot);

            display.eraseKey(result, slot);

            break;
        }
        case Operation::NONE:
        default:
            display.commandError(command);
            return 1;
    }

    return 0;
}