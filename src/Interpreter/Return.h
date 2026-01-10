#ifndef CIPR_RETURN_H
#define CIPR_RETURN_H

#include <exception>
#include "../Token/Token.h"

struct Return final : std::exception {
    const Literal value;

    explicit Return(Literal value) : value(std::move(value)) {}
};

#endif //CIPR_RETURN_H
