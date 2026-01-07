//
// Created by creed on 1/4/26.
//

#include "Core.h"
#include "../Scanner/Scanner.h"
#include "../AST/AstPrinter/AstPrinter.h"
#include "../Parser/Parser.h"
#include "../Interpreter/Interpreter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

bool Core::hadError = false;

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
    std::string line;
    for (;;) {
        std::cout << "> ";
        if (!std::getline(std::cin, line))
            break;
        run(line);
        hadError = false;
    }
}

void Core::run(const std::string& source) {
    Scanner scanner(source);
    const std::vector<Token> tokens = scanner.scanTokens();

    Arena arena;
    Parser parser(tokens, arena);
    const int rootIndex = parser.parse();

    if (hadError)
        return;

    Interpreter interpreter(arena);
    interpreter.interpret(rootIndex);
}

void Core::error(const int line, const std::string& message) {
    report(line, "", message);
}

void Core::report(const int line, const std::string& where, const std::string& message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
    hadError = true;
}