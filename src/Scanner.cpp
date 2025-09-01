#include <unordered_map>
#include "../include/TokenType.hpp"
#include "../include/Lox.hpp"
#include "../include/Token.hpp"
#include "../include/Scanner.hpp"
#include <vector>
#include <stdexcept> // for std::out_of_range
#include <type_traits>

template <typename T>
constexpr bool is_string_v = std::is_same_v<T, std::string>;

using Map = std::unordered_map<std::string, TokenType>;
/**
    Scanner Constructor
**/
Scanner::Scanner(std::string source_) : source{source_}
{
}
/**
    Scans for tokens and places it in token vector
**/
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
/**
    Check if we have reach end of the code or not
**/
bool Scanner::isAtEnd()
{
    return current >= source.length();
}
/**
    Creates Token
**/
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
        else if (match('*'))
        {
            advance();
            while (peek() != '*' && !isAtEnd())
            {
                advance();
            }

            if (isAtEnd())
            {
                break;
            }
            advance();
            if (peek() == '/')
            {
                advance();
                break;
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
    case '"':
        string();
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
/**
    Returns the string
    eg:
    "hello world"
    then it returns
    -> Hello world
**/
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
    std::string value = source.substr(start + 1, current - start - 1);
    advance();
    addToken(TokenType::STRING, value);
}
/**
    Returns the character after current pointed character
**/
char Scanner::peekNext()
{
    if (current + 1 >= source.length())
        return '\0';
    return source.at(current + 1);
}
/**
    Creates an number Token
**/
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
             std::atof(source.substr(start, current - start).c_str()));
}
/**
    Checks if the lexeme is identifier or not

**/
void Scanner::identifier()
{
    while (isAlphaNumeric(peek()))
        advance();

    std::string text = source.substr(start, current - start);
    // advance();
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

/**
    Checks if the Character is numberic or not
**/
bool Scanner::isDigit(char c)
{
    return c >= '0' && c <= '9';
}
/**
 *    Return the current pointed character
 **/
char Scanner::peek()
{
    if (isAtEnd())
        return '\0';
    return source.at(current);
}
/**
    Returns the character pointed by current and increasese the current
**/
char Scanner::advance()
{
    return source.at(current++);
}
/**
    Wrapper arounf AddToken that only takes TokenType
**/
void Scanner::addToken(TokenType type)
{
    addToken(type, NULL);
}
/**
    Checks if the character is alphabet or not
**/
bool Scanner::isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}
/**
    Checks if the character is Alphavet or Number
**/
bool Scanner::isAlphaNumeric(char c)
{
    return isAlpha(c) || isDigit(c);
}
/**
    Add the token to tokens vector
**/
void Scanner::addToken(TokenType type, std::any literal)
{
    if (literal.type() == typeid(std::string))
    {
        std::string text = source.substr(start + 1, current - start - 1);

        tokens.emplace_back(type, text, literal, line);
        return;
    }
    std::string text = source.substr(start, current - start);

    tokens.emplace_back(type, text, NULL, line);
}

/**
    if the char pointed by current is same asexpected return true else false
**/
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
