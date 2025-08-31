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

        case '!':
            addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;
        case '=':
            addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case '<':
            addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;
        case '>':
            addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;

        case '/':
            if (match('/'))
            {
                while (peek() != '\n' && !isAtEnd())
                {
                    advance();
                }
            }
            else
            {
                addToken(TokenType::SLASH);
            }
        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace.
            break;

        case '\n':
            line++;
            break;
        default:
            if (isDigit(c))
            {
                number();
            }
            else
            {
                Lox::error(line, "Unexpected character.");
            }
            break;
        }
    }

    void string()
    {
        while (peek() != '"' && !isAtEnd())
        {
            if (peek() == '\n')
                line++;
            advance();
        }
        if (isAtEnd())
        {
            Lox::error(line, "Unterminated string.");
            return;
        }
        advance();
        std::string value = source.substr(start + 1, current - 1);
        addToken(TokenType::STRING, value);
    }

    char peekNext()
    {
        if (current + 1 >= source.length())
            return '\0';
        return source.at(current + 1);
    }

    void number()
    {
        while (isDigit(peek()))
            advance();

        // Look for a fractional part.
        if (peek() == '.' && isDigit(peekNext()))
        {
            // Consume the "."
            advance();

            while (isDigit(peek()))
                advance();
        }

        addToken(TokenType::NUMBER,
                 std::atof(source.substr(start, current).c_str()));
    }
    bool isDigit(char c)
    {
        return c >= '0' && c <= '9';
    }
    char peek()
    {
        if (isAtEnd())
            return '\0';
        return source.at(current);
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

    bool match(char expected)
    {
        if (isAtEnd())
            return false;
        if (source.at(current) != expected)
            return false;

        current++;
        return true;
    }

    std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;
};