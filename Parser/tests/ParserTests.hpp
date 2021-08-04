#ifndef __PARSERTESTS_H__
#define __PARSERTESTS_H__

#include "Parser/include/Parser.h"
#include "Parser/include/Declarations.h"
#include "Parser/tests/BlockTest.hpp"
#include "Parser/tests/NumbersTest.hpp"
#include "Parser/tests/BinaryOpsTest.hpp"
#include <assert.h>

class ParserTests {

    private:
        NumbersTest numberTets;
        BlockTest blockTest;
        BinaryOpsTest binaryOpsTest;

    public:
        void run() {
            numberTets.runTests();
            blockTest.runTests();
            binaryOpsTest.runTests();
        }
};

#endif // __PARSERTESTS_H__