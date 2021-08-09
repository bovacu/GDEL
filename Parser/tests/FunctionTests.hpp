#ifndef __FUNCTIONTESTS_H__
#define __FUNCTIONTESTS_H__

#include "Parser/include/Parser.hpp"
#include <assert.h>

class FunctionTests {
    public:
        void runTests() {
            test1();
            test2();
            test3();
            test4();
            std::cout << "\033[1;32m" << "✔" << " All unit test passed for Functions!" << "\033[0m\n" << std::endl;
        }

    private:
        void test1() {
            const char* _code1 = R"(
                func square(x) {
                   ret x * x;
               }
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "body": {
                                "body": [
                                    {
                                        "arguments": {
                                            "left": {
                                                "type": "ID",
                                                "value": "x"
                                            },
                                            "operator": {
                                                "type": "*",
                                                "value": "*"
                                            },
                                            "right": {
                                                "type": "ID",
                                                "value": "x"
                                            },
                                            "type": "BINARY_OP"
                                        },
                                        "type": "RET_STATEMENT"
                                    }
                                ],
                                "type": "BLOCK_STATEMENT"
                            },
                            "name": {
                                "type": "ID",
                                "value": "square"
                            },
                            "params": [
                                {
                                    "type": "ID",
                                    "value": "x"
                                }
                            ],
                            "type": "FUNCTION_DECLARATION"
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
            std::cout << "\033[1;32m" << "  ✔" << " Functions 1!" << "\033[0m\n" << std::endl;
        }

        void test2() {
            const char* _code1 = R"(
                func square() {
                   # just an empty function
               }
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "body": {
                                "body": [],
                                "type": "BLOCK_STATEMENT"
                            },
                            "name": {
                                "type": "ID",
                                "value": "square"
                            },
                            "params": [],
                            "type": "FUNCTION_DECLARATION"
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
            std::cout << "\033[1;32m" << "  ✔" << " Functions 2!" << "\033[0m\n" << std::endl;
        }

        void test3() {
            const char* _code1 = R"(
                func square(x = 4, y = -5.5, z = false, w = 'hello') {
                   ret;
                }
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "body": {
                                "body": [
                                    {
                                        "arguments": null,
                                        "type": "RET_STATEMENT"
                                    }
                                ],
                                "type": "BLOCK_STATEMENT"
                            },
                            "name": {
                                "type": "ID",
                                "value": "square"
                            },
                            "params": [
                                {
                                    "left": {
                                        "type": "ID",
                                        "value": "x"
                                    },
                                    "operator": {
                                        "type": "=",
                                        "value": "="
                                    },
                                    "right": {
                                        "type": "INTEGER",
                                        "value": "4"
                                    },
                                    "type": "ASSIGMENT_EXPRESSION"
                                },
                                {
                                    "left": {
                                        "type": "ID",
                                        "value": "y"
                                    },
                                    "operator": {
                                        "type": "=",
                                        "value": "="
                                    },
                                    "right": {
                                        "argument": {
                                            "type": "FLOAT",
                                            "value": "5.5"
                                        },
                                        "operator": {
                                            "type": "-",
                                            "value": "-"
                                        },
                                        "type": "UNARY_OP"
                                    },
                                    "type": "ASSIGMENT_EXPRESSION"
                                },
                                {
                                    "left": {
                                        "type": "ID",
                                        "value": "z"
                                    },
                                    "operator": {
                                        "type": "=",
                                        "value": "="
                                    },
                                    "right": {
                                        "type": "BOOL",
                                        "value": "false"
                                    },
                                    "type": "ASSIGMENT_EXPRESSION"
                                },
                                {
                                    "left": {
                                        "type": "ID",
                                        "value": "w"
                                    },
                                    "operator": {
                                        "type": "=",
                                        "value": "="
                                    },
                                    "right": {
                                        "type": "STRING",
                                        "formattings": [],
                                        "value": "hello"
                                    },
                                    "type": "ASSIGMENT_EXPRESSION"
                                }
                            ],
                            "type": "FUNCTION_DECLARATION"
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
            std::cout << "\033[1;32m" << "  ✔" << " Functions 3!" << "\033[0m\n" << std::endl;
        }

        void test4() {
            const char* _code1 = R"(
                func myFunc(x = 24, y = 4.4, z = false, s = 'hello') {
                   loop(z) {

                   }

                   ret x * y;
               }
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "body": {
                                "body": [
                                    {
                                        "condition": {
                                            "type": "ID",
                                            "value": "z"
                                        },
                                        "loopBody": {
                                            "body": [],
                                            "type": "BLOCK_STATEMENT"
                                        },
                                        "type": "LOOP_STATEMENT"
                                    },
                                    {
                                        "arguments": {
                                            "left": {
                                                "type": "ID",
                                                "value": "x"
                                            },
                                            "operator": {
                                                "type": "*",
                                                "value": "*"
                                            },
                                            "right": {
                                                "type": "ID",
                                                "value": "y"
                                            },
                                            "type": "BINARY_OP"
                                        },
                                        "type": "RET_STATEMENT"
                                    }
                                ],
                                "type": "BLOCK_STATEMENT"
                            },
                            "name": {
                                "type": "ID",
                                "value": "myFunc"
                            },
                            "params": [
                                {
                                    "left": {
                                        "type": "ID",
                                        "value": "x"
                                    },
                                    "operator": {
                                        "type": "=",
                                        "value": "="
                                    },
                                    "right": {
                                        "type": "INTEGER",
                                        "value": "24"
                                    },
                                    "type": "ASSIGMENT_EXPRESSION"
                                },
                                {
                                    "left": {
                                        "type": "ID",
                                        "value": "y"
                                    },
                                    "operator": {
                                        "type": "=",
                                        "value": "="
                                    },
                                    "right": {
                                        "type": "FLOAT",
                                        "value": "4.4"
                                    },
                                    "type": "ASSIGMENT_EXPRESSION"
                                },
                                {
                                    "left": {
                                        "type": "ID",
                                        "value": "z"
                                    },
                                    "operator": {
                                        "type": "=",
                                        "value": "="
                                    },
                                    "right": {
                                        "type": "BOOL",
                                        "value": "false"
                                    },
                                    "type": "ASSIGMENT_EXPRESSION"
                                },
                                {
                                    "left": {
                                        "type": "ID",
                                        "value": "s"
                                    },
                                    "operator": {
                                        "type": "=",
                                        "value": "="
                                    },
                                    "right": {
                                        "type": "STRING",
                                        "formattings": [],
                                        "value": "hello"
                                    },
                                    "type": "ASSIGMENT_EXPRESSION"
                                }
                            ],
                            "type": "FUNCTION_DECLARATION"
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
            std::cout << "\033[1;32m" << "  ✔" << " Functions 4!" << "\033[0m\n" << std::endl;
        }
};

#endif // __FUNCTIONTESTS_H__