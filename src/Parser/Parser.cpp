//
// Created by creed on 1/5/26.
//

#include "Parser.h"

#include <iostream>
#include <ostream>
#include "../Core/Core.h"

int Parser::parse() {
    std::vector<int> statements;

    while (!isAtEnd()) {
        statements.push_back(statement());
    }

    return arena.addNode(NodeType::STMT_LIST, previous(), previous().literal, statements);
}

int Parser::statement() {
    if (match({ECHO})) {
        return echoStatement();
    }

    return expressionStatement();
}

int Parser::echoStatement() {
    int expr = expression();
    consume(SEMICOLON, "Expected ';' after value");

    return arena.addNode(NodeType::STMT_ECHO, previous(), previous().literal, {expr});
}

int Parser::expressionStatement() {
    int expr = expression();
    consume(SEMICOLON, "Expected ';' after value");

    return arena.addNode(NodeType::STMT_EXPR, previous(), previous().literal, {expr});
}

int Parser::expression() {
    return equality();
}

int Parser::equality() {
    int leftIndex = comparison();

    while (match({BANG_EQUAL, EQUAL_EQUAL})) {
        const Token op = previous();
        const int rightIndex = comparison();

        leftIndex = arena.addNode(NodeType::BINARY, op, std::monostate{},
            {leftIndex, rightIndex});
    }

    return leftIndex;
}

int Parser::comparison() {
    int leftIndex = term();

    while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
        const Token op = previous();
        const int rightIndex = term();

        leftIndex = arena.addNode(NodeType::BINARY, op, std::monostate{},
            {leftIndex, rightIndex});
    }

    return leftIndex;
}

int Parser::term() {
    int leftIndex = factor();

    while (match({PLUS, MINUS})) {
        const Token op = previous();
        const int rightIndex = factor();

        leftIndex = arena.addNode(NodeType::BINARY, op, std::monostate{},
            {leftIndex, rightIndex});
    }

    return leftIndex;
}

int Parser::factor() {
    int leftIndex = unary();

    while (match({SLASH, STAR})) {
        const Token op = previous();
        const int rightIndex = unary();

        leftIndex = arena.addNode(NodeType::BINARY, op, std::monostate{},
            {leftIndex, rightIndex});
    }

    return leftIndex;
}

int Parser::unary() {
    if (match({BANG, MINUS})) {
        const Token op = previous();
        const int rightIndex = unary();

        return arena.addNode(NodeType::UNARY, op, std::monostate{},
            {rightIndex});
    }

    return primary();
}

int Parser::primary() {
    if (match({FALSE}))
        return arena.addNode(NodeType::LITERAL, previous(), false, {});
    if (match({TRUE}))
        return arena.addNode(NodeType::LITERAL, previous(), true, {});
    if (match({TOK_NULL}))
        return arena.addNode(NodeType::LITERAL, previous(), std::monostate{}, {});

    if (match({NUMBER, STRING})) {
        return arena.addNode(NodeType::LITERAL, previous(), previous().literal, {});
    }

    if (match({LEFT_PAREN})) {
        const int expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression.");

        return arena.addNode(NodeType::GROUPING, previous(), std::monostate{}, {expr});
    }

    throw error(peek(), "Expect expression.");
}

bool Parser::match(const std::initializer_list<TokenType> types) {
    for (const TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(const TokenType type) const {
    if (isAtEnd())
        return false;
    return peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd())
        current++;
    return previous();
}

bool Parser::isAtEnd() const {
    return peek().type == EOF_TOKEN;
}

Token Parser::peek() const {
    return tokens[current];
}

Token Parser::previous() const {
    return tokens[current - 1];
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type))
        return advance();

    throw error(peek(), message);
}

Parser::ParseError Parser::error(const Token& token, const std::string& message) {
    std::cerr << "[line " << token.line << "] Error at '" << token.lexeme << "': " << message << std::endl;
    Core::hadError = true;
    return {};
}