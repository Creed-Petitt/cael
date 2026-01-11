#ifndef CIPR_NATIVE_FILE_H
#define CIPR_NATIVE_FILE_H

#include "Interpreter/Callable.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>

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
    std::string toString() override { return "<native fn write_file>"; }
};

struct NativeLs final : Callable {
    int arity() override {
        return 1;
    }

    Literal call(Interpreter&, const std::vector<Literal> args) override {
        std::string path = ".";
        if (std::holds_alternative<std::string>(args[0])) {
            path = std::get<std::string>(args[0]);
        }
        auto list = std::make_shared<LiteralVector>();
        try {
            for (const auto& entry : std::filesystem::directory_iterator(path)) {
                list->elements.emplace_back(entry.path().filename().string());
            }
        } catch (...) {
            return std::monostate{};
        }
        return list;
    }

    std::string toString() override {
        return "<native fn ls>";
    }

};

#endif

    
