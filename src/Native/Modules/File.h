#ifndef CIPR_NATIVE_FILE_H
#define CIPR_NATIVE_FILE_H

#include "Interpreter/Callable.h"
#include <fstream>
#include <sstream>

struct NativeReadFile final : Callable {
    int arity() override {
        return 1;
    }

    Literal call(Interpreter&, std::vector<Literal> args) override {
        if (!std::holds_alternative<std::string>(args[0]))
            return std::monostate{};
        std::ifstream file(std::get<std::string>(args[0]));
        if (!file.is_open())
            return std::string("Error: Open failed");
        std::stringstream buf;
        buf << file.rdbuf();
        return buf.str();
    }

    std::string toString() override {
        return "<native fn read_file>";
    }
};

struct NativeWriteFile final : Callable {
    int arity() override {
        return 2;
    }

    Literal call(Interpreter&, const std::vector<Literal> args) override {
        if (!std::holds_alternative<std::string>(args[0]) || !std::holds_alternative<std::string>(args[1]))
            return false;
        std::ofstream file(std::get<std::string>(args[0]));
        if (!file.is_open())
            return false;
        file << std::get<std::string>(args[1]);
        return true;
    }

    std::string toString() override {
        return "<native fn write_file>";
    }
};

#endif
