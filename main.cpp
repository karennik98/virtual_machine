#include "./include/lexer.hpp"
#include "./include/parser.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>



int main()
{
    std::string str = "mov r1, [5469]";
    vm::lexer le(str);
    auto output = le.lex();
    for(const auto it : output)
    {
        std::cout<<int(it.type)<<" "<<it.str<<std::endl;
    }
    vm::parser par(output);
    const auto opcodes = par.parse();
    for(const auto it : opcodes)
    {
        std::cout<<it<<std::endl;
    }
    return 0;
}