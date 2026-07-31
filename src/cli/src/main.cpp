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

        case Command::INJECT_KEY:

            break;

        case Command::UPDATE_KEY:

            break;

        case Command::GET_KEY:

            break;

        case Command::ERASE_KEY:

            break;
    }

    return 0;
}