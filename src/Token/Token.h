//
// Created by creed on 1/4/26.
//

#ifndef CIPR_TOKEN_H
#define CIPR_TOKEN_H
#include <string>
#include <utility>
#include <variant>
#include <memory>
#include <vector>

struct Callable;
struct LiteralVector;

enum TokenType {
    // Single-character
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET, RIGHT_BRACKET,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR, DOLLAR,

    // One or two character
    BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,

    // Literals
    IDENTIFIER, STRING, NUMBER,

    // Keywords
    AND, CLASS, ELSE, FALSE, FN, FOR, IF, TOK_NULL, OR,
    ECHO, RETURN, SUPER, THIS, TRUE, LET, WHILE,

    EOF_TOKEN
};

using Literal = std::variant<
    std::monostate,
    std::string,
    double,
    bool,
    std::shared_ptr<Callable>,
    std::shared_ptr<LiteralVector>
>;

struct LiteralVector {
    std::vector<Literal> elements;
};

struct Token {
    const TokenType type;
    const std::string lexeme;
    const Literal literal;
    const int line;

    Token (const TokenType type, std::string lexeme, Literal literal, const int line) :
        type(type), lexeme(std::move(lexeme)), literal(std::move(literal)), line(line) {}

    std::string toString() const;
};

#endif //CIPR_TOKEN_H