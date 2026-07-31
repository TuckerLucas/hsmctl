#include "cli_display.hpp"
#include "cli_parser.hpp"

int main(int argc, const char* argv[])
{
    cli_parser parser;
    cli_display display;

    Command command = parser.parse_cmdline(argc, argv);

    switch (command)
    {
        case Command::HELP:

            display.display_help();

            break;

        case Command::STATUS:

            break;
            
        case Command::UNKNOWN:

        default:
            
            break;
    }

    return 0;
}