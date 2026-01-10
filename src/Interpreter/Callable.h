#ifndef CIPR_CALLABLE_H
#define CIPR_CALLABLE_H

#include <vector>
#include <string>
#include "Token/Token.h"

class Interpreter;

struct Callable {
    virtual ~Callable() = default;

    virtual int arity() = 0;

    virtual Literal call(Interpreter& interpreter, std::vector<Literal> arguments) = 0;

    virtual std::string toString() = 0;
};

#endif //CIPR_CALLABLE_H
