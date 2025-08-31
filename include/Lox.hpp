#pragma once
#include <iostream>
#include <string>
class Lox
{
public:
    static void error(int line, std::string message);
    int main(int argc, char **argv);
    static bool hasError;

private:
    void runPrompt();
    void runFile(std::string path);
    void run(std::string &content);

    static void report(int line, std::string where, std::string message);
};