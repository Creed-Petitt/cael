//
// Created by creed on 1/7/26.
//

#include "Environment.h"

#include "Common/RuntimeError.h"

void Environment::define(const std::string& name, const Literal& value) {
    values[name] = value;
}

Literal Environment::get(const Token& name) {
    const auto it = values.find(name.lexeme);
    if (it != values.end()) {
        return it->second;
    }

    if (enclosing != nullptr) {
       return enclosing->get(name);
    }

    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

void Environment::assign(const Token& name, const Literal &value) {
    const auto it = values.find(name.lexeme);
    if (it != values.end()) {
        it->second = value;
        return;
    }

    if (enclosing != nullptr) {
        enclosing->assign(name, value);
        return;
    }

    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}
