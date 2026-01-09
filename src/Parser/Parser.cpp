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
        statements.push_back(declaration());
    }

    return arena.addNode(NodeType::STMT_LIST, previous(), previous().literal, statements);
}

std::vector<int> Parser::block() {
    std::vector<int> statements;

    while (!check(RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }

    consume(RIGHT_BRACE, "Expect '}' after block.");

    return statements;
}

int Parser::declaration() {
    try {
        if (match({LET})) {
            return varDeclaration();
        }
        return statement();
    } catch ([[maybe_unused]] const ParseError& e) {
        synchronize();
        return -1;
    }
}

int Parser::statement() {
    if (match({ECHO})) {
        return echoStatement();
    }

    if (match({IF})) {
        return ifStatement();
    }

    if (match({WHILE})) {
        return whileStatement();
    }

    if (match({LEFT_BRACE})) {
        const std::vector<int> statements = block();
        return arena.addNode(NodeType::STMT_BLOCK, previous(), std::monostate{}, statements);
    }

    return expressionStatement();
}

int Parser::varDeclaration() {
    const Token name = consume(IDENTIFIER, "Expect variable name.");

    int initializer = -1;
    if (match({EQUAL})) {
        initializer = expression();
    }

    consume(SEMICOLON, "Expected ';' after declaration");

    return arena.addNode(NodeType::STMT_VAR_DECL, name, std::monostate{}, {initializer});
}

int Parser::consumeBlock(const std::string& errorMessage) {
    if (match({LEFT_BRACE})) {
        const std::vector<int> statements = block();
        return arena.addNode(NodeType::STMT_BLOCK, previous(), std::monostate{},
            statements);
    }
    throw error(peek(), errorMessage);
}

int Parser::consumeCondition(const std::string& name) {
    consume(LEFT_PAREN, "Expect '(' after '" + name + "' statement.");
    const int condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after " + name + " condition.");
    return condition;
}
int Parser::whileStatement() {
    int condition = consumeCondition("while");
    int body = consumeBlock("Expect '{' after 'while'");

    return arena.addNode(NodeType::STMT_WHILE, previous(), std::monostate{},
        {condition, body});
}

int Parser::ifStatement() {
    int condition = consumeCondition("if");

    int thenBranch = consumeBlock("Expect '{' after if condition.");

    int elseBranch = -1;
    if (match({ELSE})) {
        elseBranch = consumeBlock("Expect '{' after else.");
    }

    return arena.addNode(NodeType::STMT_IF, previous(), std::monostate{},
        {condition, thenBranch, elseBranch});
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
    return assignment();
}

int Parser::assignment() {
    const int expr = logical_or();

    if (match({EQUAL})) {
        const Token equals = previous();

        int value = assignment();

        const Node& leftNode = arena.get(expr);

        if (leftNode.type == NodeType::VAR_EXPR) {
            const Token name= leftNode.op;

            return arena.addNode(NodeType::ASSIGN, name, std::monostate{}, {value});
        }

        error(equals, "Invalid assignment target.");
    }

    return expr;
}

int Parser::logical_or() {
    int left = logical_and();

    while (match({OR})) {
        const Token op = previous();
        int right = logical_and();
        left = arena.addNode(NodeType::LOGICAL, op, std::monostate{},
            {left, right});
    }

    return left;
}

int Parser::logical_and() {
    int left = equality();

    while (match({AND})) {
        const Token op = previous();
        int right = equality();
        left = arena.addNode(NodeType::LOGICAL, op, std::monostate{},
            {left, right});
    }

    return left;
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

    if (match({IDENTIFIER})) {
        return arena.addNode(NodeType::VAR_EXPR, previous(), previous().literal, {});
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

void Parser::synchronize() {
    advance();

    while (!isAtEnd()) {
        if (previous().type == SEMICOLON) return;

        switch (peek().type) {
            case CLASS:
            case FN:
            case LET:
            case FOR:
            case IF:
            case WHILE:
            case ECHO:
            case RETURN:
                return;
            default:
                break;
        }

        advance();
    }
}