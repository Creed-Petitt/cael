#ifndef CIPR_FUNCTION_H
#define CIPR_FUNCTION_H

#include "Interpreter/Callable.h"
#include <memory>
#include <vector>
#include <string>

class Interpreter;
class Environment;
class Arena;
struct Node;

class Function final : public Callable {
public:
    Function(int declarationIdx, Arena& arena, std::shared_ptr<Environment> closure);

    int arity() override;
    Literal call(Interpreter& interpreter, std::vector<Literal> arguments) override;
    std::string toString() override;

private:
    int declarationIdx;
    Arena& arena;
    std::shared_ptr<Environment> closure;
};

#endif //CIPR_FUNCTION_H