#ifndef __STRINGTESTS_H__
#define __STRINGTESTS_H__

#include "Parser/include/Parser.hpp"
#include <assert.h>

class StringTests {
    public:
        void runTests() {
            runAllTests();
            std::cout << "\033[1;32m" << "✔" << " All unit test passed for Strings!" << "\033[0m\n" << std::endl;
        }

    private:
        void runAllTests() {
            test1();
        }

        void test1() {
            const char* _code1 = R"(
                'hellooo';
                var _z = 'hello \'escapado\' oh yeah';
                var y = 'hello {'formatted'} with any value {-123} nice';
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "expression": {
                                "formattings": [],
                                "type": "STRING",
                                "value": "hellooo"
                            },
                            "type": "EXPRESSION_STATEMENT"
                        },
                        {
                            "declarations": [
                                {
                                    "id": {
                                        "type": "ID",
                                        "value": "_z"
                                    },
                                    "init": {
                                        "formattings": [],
                                        "type": "STRING",
                                        "value": "hello 'escapado' oh yeah"
                                    },
                                    "type": "VARIABLE_DECLARATION"
                                }
                            ],
                            "type": "VARIABLE_STATEMENT"
                        },
                        {
                            "declarations": [
                                {
                                    "id": {
                                        "type": "ID",
                                        "value": "y"
                                    },
                                    "init": {
                                        "formattings": [
                                            {
                                                "body": {
                                                    "formattings": [],
                                                    "type": "STRING",
                                                    "value": "formatted"
                                                },
                                                "index": 0,
                                                "type": "FORMAT_EXPRESSION"
                                            },
                                            {
                                                "body": {
                                                    "argument": {
                                                        "type": "INTEGER",
                                                        "value": "123"
                                                    },
                                                    "operator": {
                                                        "type": "-",
                                                        "value": "-"
                                                    },
                                                    "type": "UNARY_OP"
                                                },
                                                "index": 1,
                                                "type": "FORMAT_EXPRESSION"
                                            }
                                        ],
                                        "type": "STRING",
                                        "value": "hello {0} with any value {1} nice"
                                    },
                                    "type": "VARIABLE_DECLARATION"
                                }
                            ],
                            "type": "VARIABLE_STATEMENT"
                        },
                        {
                            "type": "EOF",
                            "value": "EOF"
                        }
                    ],
                    "type": "PROGRAM"
                }
            )";
            _resultAst = parse(_code1);
            assert(json::parse(_expectedAst) == _resultAst);
            std::cout << "\033[1;32m" << "  ✔" << " Strings 1!" << "\033[0m\n" << std::endl;
        }
};

#endif // __STRINGTESTS_H__