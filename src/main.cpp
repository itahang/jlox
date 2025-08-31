#include <iostream>
#include <fstream>
#include <sstream>
#include <string_view>
#include <vector>
#include <any>
#include <list>
#include "Tokens.cpp"
#include "Lox.cpp"

int main(int argc, char **argv)
{
    Lox main;
    main.hasError=false;
    int errorC = main.main(argc, argv);
    return errorC;
}