#include "cli_parser.hpp"

int main(int argc, const char* argv[])
{
    cli_parser parser;

    parser.parse_cmdline(argc, argv);

    return 0;
}