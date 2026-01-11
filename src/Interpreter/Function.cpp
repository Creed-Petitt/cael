#include "Function.h"
#include "Interpreter.h"
#include "Common/Return.h"
#include "AST/Node.h"
#include "Environment/Environment.h"

Function::Function(int declarationIdx, Arena& arena, std::shared_ptr<Environment> closure)
    : declarationIdx(declarationIdx), arena(arena), closure(std::move(closure)) {}

int Function::arity() {
    const Node& decl = arena.get(declarationIdx);
    return static_cast<int>(decl.children.size()) - 1;
}

Literal Function::call(Interpreter& interpreter, const std::vector<Literal> arguments) {
    const auto environment = std::make_shared<Environment>(closure);
    const Node& decl = arena.get(declarationIdx);

    for (size_t i = 0; i < arguments.size(); ++i) {
        const Node& param = arena.get(decl.children[i]);
        environment->define(param.op.lexeme, arguments[i]);
    }

    const Node& body = arena.get(decl.children.back());
    try {
        interpreter.executeBlock(body.children, environment);
    } catch (const Return& returnValue) {
        return returnValue.value;
    }

    return std::monostate{};
}

std::string Function::toString() {
    return "<fn " + arena.get(declarationIdx).op.lexeme + ">";
}
