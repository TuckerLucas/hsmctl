#include "cli_display.hpp"

#include <iostream>

void cli_display::display_help()
{
    std::cout << "\n";
    std::cout << "hsmctl - HSM key management and audit logging\n";
    std::cout << "\n";
    std::cout << "Usage:\n";
    std::cout << "  hsmctl <command> [flags]\n";
    std::cout << "\n";
    std::cout << "Global flags:\n";
    std::cout << "  --help       Show this help menu\n";
    std::cout << "\n";
}