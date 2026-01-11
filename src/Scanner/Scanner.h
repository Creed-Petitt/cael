//
// Created by creed on 1/4/26.
//

#ifndef CIPR_SCANNER_H
#define CIPR_SCANNER_H
#include <vector>
#include <unordered_map>

#include "../Token/Token.h"

class Scanner {
public:
    explicit Scanner(std::string  source);
    std::vector<Token> scanTokens();

private:
    const std::string source_;
    std::vector<Token> tokens;
    static const std::unordered_map<std::string, TokenType> keywords;
    int start = 0;
    int current = 0;
    int line = 1;

    bool isAtEnd() const;

    void scanToken();

    char advance() ;

    void addToken(TokenType type);

    void addToken(TokenType type, const Literal& literal);

    bool match(char expected);

    char peek() const;

    char peekNext() const;

    void string(char delimiter);

    static bool isDigit(char c) ;

    void number() ;

    static bool isAlpha(char c);

    static bool isAlphaNumeric(char c);

    void identifier();
};

#endif //CIPR_SCANNER_H