//
// Created by creed on 1/4/26.
//

#include "Scanner.h"

#include <utility>
#include "../Core/Core.h"

const std::unordered_map<std::string, TokenType> Scanner::keywords = {
    {"and",    AND},
    {"class",  CLASS},
    {"else",   ELSE},
    {"false",  FALSE},
    {"for",    FOR},
    {"fn",     FN},
    {"if",     IF},
    {"nil",    NIL},
    {"or",     OR},
    {"print",  PRINT},
    {"return", RETURN},
    {"super",  SUPER},
    {"this",   THIS},
    {"true",   TRUE},
    {"let",    LET},
    {"while",  WHILE}
};

Scanner::Scanner(std::string source)
    : source_(std::move(source)) {}

std::vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }

    tokens.emplace_back(EOF_TOKEN, "", std::monostate{}, line);
    return tokens;
}

bool Scanner::isAtEnd() const {
    return current >= source_.length();
}

char Scanner::advance()  {
    return source_[current++];
}

void Scanner::addToken(const TokenType type) {
    addToken(type, std::monostate{});
}

void Scanner::addToken(TokenType type, const Literal& literal) {
    std::string text = source_.substr(start, current - start);

    tokens.emplace_back(type, text, literal, line);
}

void Scanner::scanToken() {
    switch (char c = advance()) {
        case '(': addToken(LEFT_PAREN); break;
        case ')': addToken(RIGHT_PAREN); break;
        case '{': addToken(LEFT_BRACE); break;
        case '}': addToken(RIGHT_BRACE); break;
        case ',': addToken(COMMA); break;
        case '.': addToken(DOT); break;
        case '-': addToken(MINUS); break;
        case '+': addToken(PLUS); break;
        case ';': addToken(SEMICOLON); break;
        case '*': addToken(STAR); break;

        case '!': addToken(match('=') ? BANG_EQUAL : BANG); break;
        case '=': addToken(match('=') ? EQUAL_EQUAL : EQUAL); break;
        case '<': addToken(match('=') ? LESS_EQUAL : LESS); break;
        case '>': addToken(match('=') ? GREATER_EQUAL : GREATER); break;

        case '/':
            if (match('/')) {
                while (peek() != '\n' && !isAtEnd()) advance();
            } else {
                addToken(SLASH);
            }
            break;

        case ' ':
        case '\r':
        case '\t':
            break;

        case '\n':
            line++;
            break;

        case '"': string(); break;

        default:
            if (isDigit(c)) {
                number();
            } else if (isAlpha(c)) {
                identifier();
            } else {
                Core::error(line, "Unexpected character.");
            }
            break;
    }
}

bool Scanner::match(char expected) {
    if (isAtEnd()) return false;
    if (source_[current] != expected) return false;

    current++; // Only move forward if it's the character we expected
    return true;
}

char Scanner::peek() const {
    if (isAtEnd()) return '\0';
    return source_[current];
}

char Scanner::peekNext() const {
    if (current + 1 >= source_.length()) return '\0';
    return source_[current + 1];
}

void Scanner::string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') line++;
        advance();
    }

    if (isAtEnd()) {
        Core::error(line, "Unterminated string.");
        return;
    }

    advance();

    std::string value = source_.substr(start + 1, current - start - 2);
    addToken(STRING, value);
}

bool Scanner::isDigit(char c)  {
    return c >= '0' && c <= '9';
}

void Scanner::number()  {
    while (isDigit(peek())) {
        advance();
    }

    if (peek() == '.' && isDigit(peekNext())) {
        advance();

        while (isDigit(peek())) advance();
    }

    double value = std::stod(source_.substr(start, current - start));
    addToken(NUMBER, value);
}

bool Scanner::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

bool Scanner::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

void Scanner::identifier() {
    while (isAlphaNumeric(peek())) advance();

    const std::string text = source_.substr(start, current - start);

    const auto it = keywords.find(text);
    const TokenType type = (it == keywords.end()) ? IDENTIFIER : it->second;

    addToken(type);
}