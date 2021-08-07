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
                import 'MathModule';
                
                struct Point {
                    func constructor(_x, _y) {
                        this.x = _x;
                        this.y = _y;
                    }

                    func getX() {
                        ret this.x;
                    }

                    func getY() {
                        ret this.y;
                    }
                }

                var _myPoint = new Point(10, 5);
                _myPoint.x = _myPoint.y;
            )");
            std::cout << _root.dump(4) << std::endl;
        } catch(CoreException e) {  }
        catch(const char *e) { std::cerr << e << std::endl; return -1; }
    #endif
	
    return 0;
}