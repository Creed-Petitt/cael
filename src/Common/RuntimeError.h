//
// Created by creed on 1/5/26.
//

#ifndef CIPR_RUNTIMEERROR_H
#define CIPR_RUNTIMEERROR_H

#include <stdexcept>
#include <string>
#include <utility>
#include "Token/Token.h"

class RuntimeError final : public std::runtime_error {
public:
    const Token token;

    RuntimeError(Token token, const std::string& message)
        : std::runtime_error(message), token(std::move(token)) {}
};

#endif //CIPR_RUNTIMEERROR_H