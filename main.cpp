#include <iostream>
#include "Parser/include/Parser.h"
#include "Parser/include/Exception.hpp"
#include "Parser/tests/ParserTests.hpp"

int main(int argc, char *argv[]) {
    
    #ifdef RUN_TESTS
        ParserTests _pt;
        _pt.run();
    #else
        Parser _p;
        try {
            auto _root = _p.parse(R"(
               loop(x > 5) {
                    if(x == 3) {
                        for(var _z in 9...10) {
                            var _string = 'stringyyy';
                        }
                    }
                }
            )");
            std::cout << _root.dump(4) << std::endl;
        } catch(CoreException e) {  }
        catch(const char *e) { std::cerr << e << std::endl; return -1; }
    #endif
	
    return 0;
}