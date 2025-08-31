#include <unordered_map>
#include "../include/TokenType.hpp"
#include "../include/Lox.hpp"
#include "../include/Token.hpp"
#include "../include/Scanner.hpp"
#include <vector>
#include <stdexcept> // for std::out_of_range

using Map = std::unordered_map<std::string, TokenType>;
Scanner::Scanner(std::string source_) : source{source_}
{
}

std::vector<Token> Scanner::scanTokens()
{
    while (!isAtEnd())
    {
        start = current;
        scanToken();
    }
    tokens.emplace_back(TokenType::EOFF, "", NULL, line);
    return tokens;
}

bool Scanner::isAtEnd()
{
    return current >= source.length();
}

void Scanner::scanToken()
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
        else if (isAlpha(c))
        {
            identifier();
        }
        else
        {
            Lox::error(line, "Unexpected character.");
        }
        break;
    }
}

void Scanner::string()
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

char Scanner::peekNext()
{
    if (current + 1 >= source.length())
        return '\0';
    return source.at(current + 1);
}

void Scanner::number()
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

void Scanner::identifier()
{
    while (isAlphaNumeric(peek()))
        advance();

    std::string text = source.substr(start, current);
    TokenType type;
    try
    {
        type = keywords.at(text);
    }
    catch (std::out_of_range &e)
    {
        type = TokenType::IDENTIFIER;
    }
    addToken(type);
}

bool Scanner::isDigit(char c)
{
    return c >= '0' && c <= '9';
}
char Scanner::peek()
{
    if (isAtEnd())
        return '\0';
    return source.at(current);
}

char Scanner::advance()
{
    return source.at(current++);
}
void Scanner::addToken(TokenType type)
{
    addToken(type, NULL);
}
bool Scanner::isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}
bool Scanner::isAlphaNumeric(char c)
{
    return isAlpha(c) || isDigit(c);
}

void Scanner::addToken(TokenType type, std::any literal)
{
    std::string text = source.substr(start, current);
    tokens.emplace_back(type, text, NULL, line);
}

bool Scanner::match(char expected)
{
    if (isAtEnd())
        return false;
    if (source.at(current) != expected)
        return false;

    current++;
    return true;
}

Map Scanner::keywords = {
    {"and", TokenType::AND},
    {"class", TokenType::CLASS},
    {"else", TokenType::ELSE},
    {"false", TokenType::FALSE},
    {"for", TokenType::FOR},
    {"fun", TokenType::FUN},
    {"if", TokenType::IF},
    {"nil", TokenType::NIL},
    {"or", TokenType::OR},
    {"print", TokenType::PRINT},
    {"return", TokenType::RETURN},
    {"super", TokenType::SUPER},
    {"this", TokenType::THIS},
    {"true", TokenType::TRUE},
    {"var", TokenType::VAR},
    {"while", TokenType::WHILE}};
