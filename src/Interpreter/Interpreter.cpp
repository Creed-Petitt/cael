//
// Created by creed on 1/5/26.
//

#include "Interpreter.h"
#include "RuntimeError.h"

void Interpreter::interpret(const int rootIndex) {
    try {
        execute(rootIndex);
    } catch (const RuntimeError& error) {
        std::cerr << "Runtime Error: " << error.what() << "\n[line " << error.token.line << "]" << std::endl;
    }
}

Literal Interpreter::evaluate(const int index) {
    if (index == -1)
        return std::monostate{};

    switch (const Node& node = arena.get(index); node.type) {
        case NodeType::LITERAL:
            return visitLiteral(node);
        case NodeType::GROUPING:
            return visitGrouping(node);
        case NodeType::UNARY:
            return visitUnary(node);
        case NodeType::BINARY:
            return visitBinary(node);
        default:
            return std::monostate{};
    }
}

Literal Interpreter::visitLiteral(const Node& node) {
    return node.value;
}

Literal Interpreter::visitGrouping(const Node& node) {
    return evaluate(node.children[0]);
}

Literal Interpreter::visitUnary(const Node& node) {
    const Literal right = evaluate(node.children[0]);

    switch (node.op.type) {
        case MINUS:
            checkNumberOperand(node.op, right);
            return -std::get<double>(right);

        case BANG:
            return !isTruthy(right);

        default:
            return std::monostate{};
    }
}

Literal Interpreter::visitBinary(const Node& node) {
    const Literal left = evaluate(node.children[0]);
    const Literal right = evaluate(node.children[1]);

    switch (node.op.type) {
        case MINUS:
            checkNumberOperands(node.op, left, right);
            return std::get<double>(left) - std::get<double>(right);
        case SLASH:
            checkNumberOperands(node.op, left, right);
            if (std::get<double>(right) == 0.0) {
                throw RuntimeError(node.op, "Division by zero.");
            }
            return std::get<double>(left) / std::get<double>(right);
        case STAR:
            checkNumberOperands(node.op, left, right);
            return std::get<double>(left) * std::get<double>(right);

        case PLUS:
            if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                return std::get<double>(left) + std::get<double>(right);
            }
            if (std::holds_alternative<std::string>(left) || std::holds_alternative<std::string>(right)) {
                return stringify(left) + stringify(right);
            }

            throw RuntimeError(node.op, "Operands must be two numbers or two strings.");

        case GREATER:
            checkNumberOperands(node.op, left, right);
            return std::get<double>(left) > std::get<double>(right);
        case GREATER_EQUAL:
            checkNumberOperands(node.op, left, right);
            return std::get<double>(left) >= std::get<double>(right);
        case LESS:
            checkNumberOperands(node.op, left, right);
            return std::get<double>(left) < std::get<double>(right);
        case LESS_EQUAL:
            checkNumberOperands(node.op, left, right);
            return std::get<double>(left) <= std::get<double>(right);

        case BANG_EQUAL: return !isEqual(left, right);
        case EQUAL_EQUAL: return isEqual(left, right);

        default: return std::monostate{};
    }
}

bool Interpreter::isTruthy(const Literal& value) {
    if (std::holds_alternative<std::monostate>(value))
        return false;

    if (std::holds_alternative<bool>(value))
        return std::get<bool>(value);

    if (std::holds_alternative<double>(value)) {
        return std::get<double>(value) != 0.0;
    }

    return true;
}

void Interpreter::checkNumberOperand(const Token& op, const Literal& operand) {
    if (std::holds_alternative<double>(operand))
        return;
    throw RuntimeError(op, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(const Token& op, const Literal& left, const Literal& right) {
    if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
        return;
    throw RuntimeError(op, "Operands must be numbers.");
}

bool Interpreter::isEqual(const Literal& a, const Literal& b) {
    if (a.index() != b.index())
        return false;
    return a == b;
}

std::string Interpreter::stringify(const Literal& value) {
    if (std::holds_alternative<std::monostate>(value)) return "null";
    if (std::holds_alternative<bool>(value)) return std::get<bool>(value) ? "true" : "false";

    if (std::holds_alternative<double>(value)) {
        std::string text = std::to_string(std::get<double>(value));
        text.erase(text.find_last_not_of('0') + 1, std::string::npos);
        if (text.back() == '.') text.pop_back();
        return text;
    }

    if (std::holds_alternative<std::string>(value)) return std::get<std::string>(value);
    return "unknown";
}

void Interpreter::execute(const int index) {
    const Node& node = arena.get(index);

    switch (node.type) {
        case NodeType::STMT_LIST:
            for (const int childIndex : node.children) {
                execute(childIndex);
            }
            break;

        case NodeType::STMT_ECHO: {
            const Literal value = evaluate(node.children[0]);
            std::cout << stringify(value) << std::endl;
            break;
        }

        case NodeType::STMT_EXPR:
            evaluate(node.children[0]);
            break;

        default:
            evaluate(index);
            break;
    }
}