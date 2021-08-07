#ifndef __IMPORTTESTS_H__
#define __IMPORTTESTS_H__

#include "Parser/include/Parser.h"
#include "Parser/include/Declarations.h"
#include <assert.h>

class ImportTests {
    public:
        void runTests() {
            test1();
            std::cout << "\033[1;32m" << "✔" << " All unit test passed for Import!" << "\033[0m\n" << std::endl;
        }

    private:
        void test1() {
            Parser _parser;
            const char* _code1 = R"(
                import 'MathModule';
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "path": {
                                "type": "STRING",
                                "value": "MathModule"
                            },
                            "type": "IMPORT_STATEMENT"
                        },
                        {
                            "type": "EOF",
                            "value": "EOF"
                        }
                    ],
                    "type": "PROGRAM"
                }
            )";
            _resultAst = _parser.parse(_code1);
            assert(json::parse(_expectedAst) == _resultAst);
        }
};

#endif // __IMPORTTESTS_H__