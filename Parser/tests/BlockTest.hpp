#ifndef __BLOCKSTEST_H__
#define __BLOCKSTEST_H__

#include "Parser/include/Parser.hpp"
#include <assert.h>

class BlockTest {
    public:
        void runTests() {
            runAllTests();
            std::cout << "\033[1;32m" << "✔" << " All unit test passed for BlockStatement!" << "\033[0m\n" << std::endl;
        }

    private:
        void runAllTests() {
            test1();
            test2();
            test3();
        }

        void test1() {
            const char* _code1 = R"(
                {}
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "body": [],
                            "type": "BLOCK_STATEMENT"
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
            std::cout << "\033[1;32m" << "  ✔" << " BlockStatement 1!" << "\033[0m\n" << std::endl;
        }

        void test2() {
            const char* _code1 = R"(
                {
                    45;
                    'blocky';
                }
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "body": [
                                {
                                    "expression": {
                                        "type": "INTEGER",
                                        "value": "45"
                                    },
                                    "type": "EXPRESSION_STATEMENT"
                                },
                                {
                                    "expression": {
                                        "type": "STRING",
                                        "value": "blocky",
                                        "formattings": []
                                    },
                                    "type": "EXPRESSION_STATEMENT"
                                }
                            ],
                            "type": "BLOCK_STATEMENT"
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
            std::cout << "\033[1;32m" << "  ✔" << " BlockStatement 2!" << "\033[0m\n" << std::endl;
        }

        void test3() {
            const char* _code1 = R"(
                {
                    45;
                    'blockyOut';
                    {
                        'blockyIn';
                        1;
                    }
                }
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "body": [
                                {
                                    "expression": {
                                        "type": "INTEGER",
                                        "value": "45"
                                    },
                                    "type": "EXPRESSION_STATEMENT"
                                },
                                {
                                    "expression": {
                                        "type": "STRING",
                                        "value": "blockyOut",
                                        "formattings": []
                                    },
                                    "type": "EXPRESSION_STATEMENT"
                                },
                                {
                                    "body": [
                                        {
                                            "expression": {
                                                "type": "STRING",
                                                "formattings": [],
                                                "value": "blockyIn"
                                            },
                                            "type": "EXPRESSION_STATEMENT"
                                        },
                                        {
                                            "expression": {
                                                "type": "INTEGER",
                                                "value": "1"
                                            },
                                            "type": "EXPRESSION_STATEMENT"
                                        }
                                    ],
                                    "type": "BLOCK_STATEMENT"
                                }
                            ],
                            "type": "BLOCK_STATEMENT"
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
            std::cout << "\033[1;32m" << "  ✔" << " BlockStatement 3!" << "\033[0m\n" << std::endl;
        }
};

#endif // __BLOCKSTEST_H__