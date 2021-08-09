#include <iostream>
// #include "Parser/include/Parser.h"
// #include "Parser/include/Exception.hpp"
#include "Parser/tests/ParserTests.hpp"

// #include "Parser/include/Parser.hpp"

int main(int argc, char *argv[]) {
    
    #ifdef RUN_TESTS
        ParserTests _pt;
        _pt.run();
    #else
        try {
            auto _root = parse(R"(
                struct Point extends BasePoint {
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

                    func print() {
                        console.log(self.x, self.y, getX(), getY(), z[0]);
                    }
                }

                var _myPoint = new Point(10, y[1]);
            )");
            std::cout << _root.dump(4) << std::endl;
        } catch(CoreException e) {  }
        catch(const char *e) { std::cerr << e << std::endl; return -1; }
    #endif
	
    return 0;
}