#pragma once
#include <iostream>

void printHelp()
{
    std::cout << "Usage: steam_uploader [options]\n";
    std::cout << "Options:\n";
    std::cout << "  -a, --appID <id>            App ID (mandatory)\n";
    std::cout << "  -w, --workshopID <id>       Workshop ID (mandatory non-new workshop item)\n";
    std::cout << "  -n, --new                   Create a new workshop item\n";
    std::cout << "\n";
    std::cout << "  -d, --description <path>    Path to the description file\n";
    std::cout << "  -p, --preview <path>        Path to the preview image\n";
    std::cout << "  -c, --content <path>        Path to the content folder\n";
    std::cout << "  -t, --title <title>         Title of the item\n";
    std::cout << "  -v, --visibility <level>    Visibility level (0-3)\n";
    std::cout << "  -T, --tags <tags>           Comma-separated list of developer predefined tags\n";
    std::cout << "\n";
    std::cout << "  -P, --patchnote <path>      Path to the patch note file\n";
    std::cout << "  -L, --language <lang>       Language code (default: english)\n";
    std::cout << "\n";
    std::cout << "  -V, --verbose               Enable verbose output (unused)\n";
    std::cout << "  -h, --help                  Show this help message\n";
}