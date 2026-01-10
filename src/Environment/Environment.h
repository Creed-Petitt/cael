//
// Created by creed on 1/7/26.
//

#ifndef CIPR_ENVIRONMENT_H
#define CIPR_ENVIRONMENT_H
#include <memory>
#include <string>
#include <unordered_map>
#include "Token/Token.h"

class Environment {
public:
    Environment() : enclosing(nullptr) {}
    explicit Environment(const std::shared_ptr<Environment> &enclosing)
        : enclosing(enclosing) {}

    void define(const std::string& name, const Literal& value);
    Literal get(const Token& name);
    void assign(const Token& name, const Literal &value);

    std::shared_ptr<Environment> enclosing;
private:
    std::unordered_map<std::string, Literal> values;
};


#endif //CIPR_ENVIRONMENT_H