#pragma once
#include <string>
#include <vector>
#include <any>
#include <unordered_map>
#include "TokenType.hpp"
#include "Token.hpp"

using Map = std::unordered_map<std::string, TokenType>;

class Scanner
{
public:
    static Map keywords;
    Scanner(std::string source_);
    std::vector<Token> scanTokens();
    
private:
    bool isAtEnd();
    void scanToken();
    void string();
    char peekNext();
    void number();
    void identifier();
    bool isDigit(char c);
    char peek();
    char advance();
    void addToken(TokenType type);
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
    void addToken(TokenType type, std::any literal);
    bool match(char expected);

    std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;
};
