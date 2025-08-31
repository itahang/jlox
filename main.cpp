#include <iostream>
#include "./include/Lox.hpp"

int main(int argc, char **argv)
{
    Lox lox;
    Lox::hasError = false;  // reset before running
    int errorCode = lox.main(argc, argv);
    return errorCode;
}
