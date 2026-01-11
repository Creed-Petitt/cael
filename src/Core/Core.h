//
// Created by creed on 1/4/26.
//

#ifndef CIPR_CORE_H
#define CIPR_CORE_H

#include "AST/Node.h"
#include "Interpreter/Interpreter.h"
#include <string>

class Core {
public:
    Core();

    void runFile(const std::string& path);
    void runPrompt();
    void run(const std::string& source);
    void loadConfig();

    static void error(int line, const std::string& message);
    static bool hadError;

private:
    Arena arena;
    Interpreter interpreter;

    static void report(int line, const std::string& where, const std::string& message);
};

#endif