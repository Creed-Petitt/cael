//
// Created by creed on 1/4/26.
//

#include "Token.h"
#include <sstream>

std::string tokenTypeName(const TokenType type) {
    static const char* names[] = {
        "LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACE", "RIGHT_BRACE", "DOLLAR",
        "COMMA", "DOT", "MINUS", "PLUS", "SEMICOLON", "SLASH", "STAR",
        "BANG", "BANG_EQUAL", "EQUAL", "EQUAL_EQUAL",
        "GREATER", "GREATER_EQUAL", "LESS", "LESS_EQUAL",
        "IDENTIFIER", "STRING", "NUMBER",
        "AND", "CLASS", "ELSE", "FALSE", "FN", "FOR", "IF", "TOK_NULL", "OR",
        "ECHO", "RETURN", "SUPER", "THIS", "TRUE", "LET", "WHILE",
        "EOF_TOKEN"
    };
    return names[type];
}
std::string Token::toString() const {
    std::string literal_str;

    if (std::holds_alternative<double>(literal)) {
        literal_str = std::to_string(std::get<double>(literal));
    } else if (std::holds_alternative<std::string>(literal)) {
        literal_str = std::get<std::string>(literal);
    } else {
        literal_str = "null";
    }
    return tokenTypeName(type) + " " + lexeme + " " + literal_str;
}