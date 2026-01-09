//
// Created by creed on 1/5/26.
//

#ifndef CIPR_INTERPRETER_H
#define CIPR_INTERPRETER_H

#include "../AST/Node/Node.h"
#include "../Environment/Environment.h"
#include <iostream>

class Interpreter {
public:
    explicit Interpreter(Arena& arena) : arena(arena) {
        environment = std::make_shared<Environment>();
    }

    void interpret(int rootIndex);

private:
    Arena& arena;
    std::shared_ptr<Environment> environment;

    Literal evaluate(int index);
    void execute(int index);
    void executeBlock(const std::vector<int>& statements,
        const std::shared_ptr<Environment> &env);

    static Literal visitLiteral(const Node& node);
    Literal visitUnary(const Node& node);
    Literal visitBinary(const Node& node);
    Literal visitGrouping(const Node& node);
    Literal visitVarExpr(const Node& node) const;
    Literal visitAssignmentExpr(const Node& node);
    Literal visitLogicalExpr(const Node& node);

    void visitBlockStmt(const Node& node);
    void visitWhileStmt(const Node& node);
    void visitIfStmt(const Node& node);
    void visitEchoStmt(const Node& node);
    void visitExpressionStmt(const Node& node);
    void visitStmtList(const Node& node);
    void visitVarDeclaration(const Node& node);


    static bool isTruthy(const Literal& value);
    static bool isEqual(const Literal& a, const Literal& b);
    static void checkNumberOperand(const Token& op, const Literal& operand);
    static void checkNumberOperands(const Token& op, const Literal& left, const Literal& right);
    static std::string stringify(const Literal& value);
};

#endif //CIPR_INTERPRETER_H