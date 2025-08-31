#include <iostream>
#include <fstream>
#include <sstream>
#include <string_view>
#include <vector>
#include <any>
#include <list>
#include "Scanner.hpp"
#include "Token.hpp"
#include "Lox.hpp"

bool Lox::hasError = false;
void Lox::run(std::string &content)
{
    Scanner scanner(content);
    auto tokens = scanner.scanTokens();

    for (Token token : tokens)
    {
        std::cout << token.lexeme << "\n";
    }
}

void Lox::error(int line, std::string message)
{
    report(line, "", message);
}

int Lox::main(int argc, char **argv)
{
    if (argc > 2)
    {
        std::cerr << "Usage: jlox [script]" << std::endl;
        exit(64);
    }
    else if (argc == 2)
    {
        runFile(std::string(argv[1]));
    }
    else
    {
        runPrompt();
    }
    return 0;
}

void Lox::runPrompt()
{
    std::string line;
    for (;;)
    {
        std::cout << ">> ";
        std::getline(std::cin, line);
        if (line == ".exit")
        {
            break;
        }
        run(line);
        hasError = false;
    }
}

void Lox::runFile(std::string path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Error while opening file\n";
        return;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string contents = buffer.str();
    run((contents));
    if (hasError)
    {
        exit(65);
    }
}

void Lox::report(int line, std::string where, std::string message)
{
    std::cerr << "\nLine [" << line << "] where: " << message<<std::endl;
    hasError = true;
}
