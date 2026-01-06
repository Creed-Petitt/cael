//
// Created by creed on 1/4/26.
//

#ifndef CIPR_TOKEN_H
#define CIPR_TOKEN_H
#include <string>
#include <utility>
#include <variant>

enum TokenType {
    // Single-character
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    // One or two character
    BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,

    // Literals
    IDENTIFIER, STRING, NUMBER,

    // Keywords
    AND, CLASS, ELSE, FALSE, FN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, LET, WHILE,

    EOF_TOKEN
};

using Literal = std::variant<std::monostate, std::string, double, bool>;

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