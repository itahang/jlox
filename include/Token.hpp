#pragma once
#include "TokenType.hpp"
#include <string>
#include <any>
struct Token
{
    TokenType type;
    std::string lexeme;
    std::any literal;
    int line;

    Token(TokenType type, const std::string &lexeme, std::any literal, int line)
        : type(type), lexeme(lexeme), literal(literal), line(line) {}
};
