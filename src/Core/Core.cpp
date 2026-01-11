//
// Created by creed on 1/4/26.
//

#include "Core.h"
#include "Scanner/Scanner.h"
#include "../AST/AstPrinter.h"
#include "Parser/Parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>

bool Core::hadError = false;

Core::Core() : interpreter(arena) {}

void Core::loadConfig() {
    const char* home = std::getenv("HOME");
    if (!home) return;

    std::string path = std::string(home) + "/.ciprrc";
    std::ifstream file(path);
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        run(buffer.str());
        hadError = false;
    }
}

void Core::runFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << path << std::endl;
        exit(74);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    run(buffer.str());

    if (hadError) exit(65);
}

void Core::runPrompt() {
    std::string buffer;
    int braceCount = 0;

    while (true) {
        if (buffer.empty()) {
            std::cout << "> ";
        } else {
            std::cout << "... ";
        }

        std::string line;
        if (!std::getline(std::cin, line)) break;

        for (const char c : line) {
            if (c == '{') braceCount++;
            if (c == '}') braceCount--;
        }

        buffer += line + "\n";

        if (braceCount <= 0 && !buffer.empty()) {
            size_t last = buffer.find_last_not_of(" \n\r\t");
            if (last != std::string::npos) {
                run(buffer);
            }
            buffer = "";
            braceCount = 0;
            hadError = false;
        }
    }
}

void Core::run(const std::string& source) {
    Scanner scanner(source);
    const std::vector<Token> tokens = scanner.scanTokens();

    Parser parser(tokens, arena);
    const int rootIndex = parser.parse();

    if (hadError)
        return;

    // Optional: Print AST only in debug mode or if requested?
    // AstPrinter printer(arena);
    // std::cout << "AST: " << printer.print(rootIndex) << std::endl;

    interpreter.interpret(rootIndex);
}

void Core::error(const int line, const std::string& message) {
    report(line, "", message);
}

void Core::report(const int line, const std::string& where, const std::string& message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
    hadError = true;
}