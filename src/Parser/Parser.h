//
// Created by creed on 1/5/26.
//

#ifndef CIPR_PARSER_H
#define CIPR_PARSER_H

#include <vector>
#include <initializer_list>
#include <stdexcept>

#include "Token/Token.h"
#include "AST/Node.h"

class Parser {
public:
    class ParseError final : public std::runtime_error {
    public:
        ParseError() : std::runtime_error("") {}
    };

    Parser(const std::vector<Token>& tokens, Arena& arena)
        : tokens(tokens), arena(arena) {}

    int parse();

private:
    const std::vector<Token>& tokens;
    Arena& arena;
    int current = 0;

    int declaration();
    int varDeclaration();
    int function(const std::string& kind);

    int consumeCondition(const std::string& name);
    std::vector<int> block();
    int consumeBlock(const std::string& errorMessage);

    int ifStatement();
    int whileStatement();
    int forStatement();
    int returnStatement();
    int statement();
    int echoStatement();
    int expressionStatement();

    int assignment();
    int expression();
    int equality();
    int logical_or();
    int logical_and();
    int comparison();
    int term();
    int factor();
    int unary();
    int call();
    int finishCall(int callee);
    int finishIndex(int callee);
    int array();
    int primary();

    void synchronize();

    bool match(std::initializer_list<TokenType> types);
    bool check(TokenType type) const;
    Token advance();
    bool isAtEnd() const;
    Token peek() const;
    Token previous() const;

    Token consume(TokenType type, const std::string& message);

    static ParseError error(const Token& token, const std::string& message);
};

#endif //CIPR_PARSER_H