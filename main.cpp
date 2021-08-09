#include <iostream>

#include "Parser/tests/ParserTests.hpp"
#include "Parser/include/Parser.hpp"

int main(int argc, char *argv[]) {
    
    #ifdef RUN_TESTS
        ParserTests _pt;
        _pt.run();
    #else
        try {
            auto _root = parse(R"(
                'hellooo';
                var _z = 'hello \'escapado\' oh yeah';
                var y = 'hello {'formatted'} with any value {-123} nice';
            )");
            std::cout << _root.dump(4) << std::endl;
        } catch(CoreException e) {  }
        catch(const char *e) { std::cerr << e << std::endl; return -1; }
    #endif
	
    return 0;
}