#include <iostream>
#include "Parser/include/Parser.h"
#include "Parser/include/Exception.hpp"
// #include "Parser/tests/ParserTests.hpp"

// void executeUnitTests(){
//     ParserTests _pt;
//     _pt.runTests();
// }

int main() {

    // executeUnitTests();

    Parser _p;
    try {
        auto _root = _p.parse(R"(
            45;
            {
                73;
                'outter block';
                {
                    'inner block';
                    54;
                }
            }
        )");
        std::cout << _root.dump(4) << std::endl;
    } catch(CoreException e) {  }
	catch(const char *e) { std::cerr << e << std::endl; return -1; }
	
    return 0;
}