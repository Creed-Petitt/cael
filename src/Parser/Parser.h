//
// Created by creed on 1/5/26.
//

#ifndef CIPR_PARSER_H
#define CIPR_PARSER_H

#include <vector>
#include <initializer_list>
#include <stdexcept>

#include "../Token/Token.h"
#include "../AST/Node/Node.h"

class Parser {
public:
    class ParseError final : public std::runtime_error {
    public:
        ParseError() : std::runtime_error("") {}
    };

    Parser(const std::vector<Token>& tokens, Arena& arena)
        : tokens(tokens), arena(arena) {
    }

    int parse();

private:
    const std::vector<Token>& tokens;
    Arena& arena;
    int current = 0;
    int statement();
    int echoStatement();
    int expressionStatement();
    int expression();
    int equality();
    int comparison();
    int term();
    int factor();
    int unary();
    int primary();

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