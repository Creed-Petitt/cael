#ifndef CIPR_NATIVE_CORE_H
#define CIPR_NATIVE_CORE_H

#include "Interpreter/Callable.h"
#include <ctime>
#include <cstdio>
#include <memory>
#include <array>
#include <string>
#include <unistd.h>
#include <climits>

struct NativeClock final : Callable {
    int arity() override {
        return 0;
    }

    Literal call(Interpreter &, std::vector<Literal>) override {
        return static_cast<double>(clock()) / CLOCKS_PER_SEC;
    }

    std::string toString() override {
        return "<native fn clock>";
    }
};

struct NativeRun final : Callable {
    int arity() override {
        return 1;
    }

    Literal call(Interpreter &, const std::vector<Literal> args) override {
        if (!std::holds_alternative<std::string>(args[0]))
            return std::monostate{};
        const auto cmd = std::get<std::string>(args[0]);
        std::array<char, 128> buf{};
        std::string res;
        const std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
        if (!pipe)
            return std::string("Error: Pipe failed");
        while (fgets(buf.data(), buf.size(), pipe.get()) != nullptr)
            res += buf.data();
        return res;
    }

    std::string toString() override {
        return "<native fn run>";
    }
};

struct NativeEnv final : Callable {
    int arity() override {
        return 1;
    }

    Literal call(Interpreter &, const std::vector<Literal> args) override {
        if (!std::holds_alternative<std::string>(args[0]))
            return std::monostate{};
        if (const char *val = std::getenv(std::get<std::string>(args[0]).c_str()))
            return std::string(val);
        return std::monostate{};
    }

    std::string toString() override { return "<native fn env>"; }
};


struct NativeCwd final : Callable {
    int arity() override {
        return 0;
    }

    Literal call(Interpreter &, std::vector<Literal>) override {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != nullptr) {
            return std::string(cwd);
        }
        return std::monostate{};
    }

    std::string toString() override {
        return "<native fn cwd>";
    }
};


struct NativeCd final : Callable {
    int arity() override {
        return 1;
    }

    Literal call(Interpreter &, const std::vector<Literal> args) override {
        if (!std::holds_alternative<std::string>(args[0]))
            return false;
        const auto path = std::get<std::string>(args[0]);
        return chdir(path.c_str()) == 0;
    }

    std::string toString() override {
        return "<native fn cd>";
    }
};

#endif
