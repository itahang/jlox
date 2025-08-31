#pragma once
#include "Tokens.cpp"
#include <vector>
#include "Lox.cpp"
class Scanner
{
public:
    Scanner(std::string source_) : source{source_}
    {
    }

    std::vector<Token> scanTokens()
    {
        while (!isAtEnd())
        {
            start = current;
            scanToken();
        }
        tokens.emplace_back(TokenType::EOFF, "", NULL, line);
        return tokens;
    }

private:
    bool isAtEnd()
    {
        return current >= source.length();
    }

    void scanToken()
    {
        char c = advance();
        switch (c)
        {
        case '(':
            addToken(TokenType::LEFT_PAREN);
            break;
        case ')':
            addToken(TokenType::RIGHT_PAREN);
            break;
        case '{':
            addToken(TokenType::LEFT_BRACE);
            break;
        case '}':
            addToken(TokenType::RIGHT_BRACE);
            break;
        case ',':
            addToken(TokenType::COMMA);
            break;
        case '.':
            addToken(TokenType::DOT);
            break;
        case '-':
            addToken(TokenType::MINUS);
            break;
        case '+':
            addToken(TokenType::PLUS);
            break;
        case ';':
            addToken(TokenType::SEMICOLON);
            break;
        case '*':
            addToken(TokenType::STAR);
            break;
        default:
            Lox::error(line, "Unexpected character.");
            break;
        }
    }

    char advance()
    {
        return source.at(current++);
    }
    void addToken(TokenType type)
    {
        addToken(type, NULL);
    }

    void addToken(TokenType type, std::any literal)
    {
        std::string text = source.substr(start, current);
        tokens.emplace_back(type, text, NULL, line);
    }

    std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;
};