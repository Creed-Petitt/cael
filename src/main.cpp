#include "Core/Core.h"
#include <iostream>

int main(const int argc, char* argv[]) {
    if (argc > 2) {
        std::cout << "Usage: core_lang [script]" << std::endl;
        return 64;
    } if (argc == 2) {
        Core::runFile(argv[1]);
    } else {
        Core::runPrompt();
    }
    return 0;
}