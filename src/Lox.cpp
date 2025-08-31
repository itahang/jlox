#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string_view>
#include <vector>
#include <any>
#include <list>
#include "Tokens.cpp"

class Lox
{
public:
    static void error(int line, std::string message)
    {
        report(line, "", message);
    }

    int main(int argc, char **argv)
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

    static bool hasError;

private:
    void runPrompt()
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

    void runFile(std::string path)
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
        run(std::move(contents));
        if (hasError)
        {
            exit(65);
        }
    }
    template <typename T = std::string>
    void run(T &&content)
    {
        std::vector<std::string> tokens;
        std::istringstream iss(content); // wrap string in a stream
        std::string token;

        while (iss >> token)
        { // now >> works
            tokens.push_back(token);
            std::cout << token << "\n";
        }
    }

    static void report(int line, std::string where, std::string message)
    {
        std::cerr << "Line [" << line << "] where: " << message;
        hasError = true;
    }
};
bool Lox::hasError = false;