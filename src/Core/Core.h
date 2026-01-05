//
// Created by creed on 1/4/26.
//

#ifndef CIPR_CORE_H
#define CIPR_CORE_H

#include <string>

class Core {
public:
    static bool hadError;

    static void runFile(const std::string& path);
    static void runPrompt();
    static void run(const std::string& source);
    static void error(int line, const std::string& message);

private:
    static void report(int line, const std::string& where, const std::string& message);
};

#endif