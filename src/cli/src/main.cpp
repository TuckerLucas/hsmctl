#include "cli_display.hpp"
#include "cli_parser.hpp"
#include "hsm_manager.hpp"
#include "SecureElement.hpp"

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

    switch (command)
    {
        case Command::HELP:
        {
            display.help();

            break;
        }
        case Command::STATUS:
        {
            SecureElement se;
            hsm_manager hsm(se);

            SecureElementStatus result = hsm.status();

            display.status(result);

            break;
        }
        case Command::UNKNOWN:
        default:
            display.unknown(argv[1]);
            break;
    }

    return 0;
}