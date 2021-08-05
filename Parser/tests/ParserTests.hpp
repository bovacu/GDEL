#ifndef __PARSERTESTS_H__
#define __PARSERTESTS_H__

#include "Parser/include/Parser.h"
#include "Parser/include/Declarations.h"
#include "Parser/tests/BlockTest.hpp"
#include "Parser/tests/NumbersTest.hpp"
#include "Parser/tests/BinaryOpsTest.hpp"
#include "Parser/tests/AssigmentTests.hpp"
#include "Parser/tests/VariablesTest.hpp"
#include "Parser/tests/IfTests.hpp"
#include <assert.h>

class ParserTests {

    private:
        NumbersTest numberTets;
        BlockTest blockTest;
        BinaryOpsTest binaryOpsTest;
        AssigmentTests assigmentTests;
        VariablesTest variablesTest;
        IfTests ifTests;

    public:
        void run() {
            numberTets.runTests();
            blockTest.runTests();
            binaryOpsTest.runTests();
            assigmentTests.runTests();
            variablesTest.runTests();
            ifTests.runTests();
        }
};

#endif // __PARSERTESTS_H__