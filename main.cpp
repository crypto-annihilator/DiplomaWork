#include <iostream>
#include "Parser.h"

int main()
{
   Parser* parser = Parser::getParser();
    //std::string line = "cos x";
   // std::string line = "13x+(eusinhuarcsinh+llnm+cos(5x)x  y * j +5.1/2)+32 + cos     x        + y + z +k*l/v";
    //std::string line = "5.1/2(xyz)";
    //std::string line = "5.1/2 + (sin(x)yz)";
   // std::string line = "5.1/2 +* (sinxyz)";
    //std::string line = "xcos(h*(h)";
    //std::string line = "x13cos(h2*(h)";
    //std::string line = "cose^y+ e^cosx ";
    //std::string line = "cos(x)^(y)";
    std::string line = "xxxxxx^(y * (z)";
    std::cout << line << std::endl;
    std::list<std::string> words;

    parser->parse(line, words);

    for (const auto& item : words) {
        std:: cout << item << "," ;
    }
    std::cout << std::endl;
    return 0;
}
