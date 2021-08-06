#ifndef __FUNCTIONTESTS_H__
#define __FUNCTIONTESTS_H__

#include "Parser/include/Parser.h"
#include "Parser/include/Declarations.h"
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
            Parser _parser;
            const char* _code1 = R"(
                func square(x) {
                   return x * x;
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
                                    "id": {
                                        "type": "ID",
                                        "value": "x"
                                    },
                                    "init": null,
                                    "type": "VARIABLE_DECLARATION"
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
            _resultAst = _parser.parse(_code1);
            assert(json::parse(_expectedAst) == _resultAst);
        }

        void test2() {
            Parser _parser;
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
            _resultAst = _parser.parse(_code1);
            assert(json::parse(_expectedAst) == _resultAst);
        }

        void test3() {
            Parser _parser;
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
                                    "id": {
                                        "type": "ID",
                                        "value": "x"
                                    },
                                    "init": {
                                        "type": "INTEGER",
                                        "value": "4"
                                    },
                                    "type": "VARIABLE_DECLARATION"
                                },
                                {
                                    "id": {
                                        "type": "ID",
                                        "value": "y"
                                    },
                                    "init": {
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
                                    "type": "VARIABLE_DECLARATION"
                                },
                                {
                                    "id": {
                                        "type": "ID",
                                        "value": "z"
                                    },
                                    "init": {
                                        "type": "BOOL",
                                        "value": "false"
                                    },
                                    "type": "VARIABLE_DECLARATION"
                                },
                                {
                                    "id": {
                                        "type": "ID",
                                        "value": "w"
                                    },
                                    "init": {
                                        "type": "STRING",
                                        "value": "hello"
                                    },
                                    "type": "VARIABLE_DECLARATION"
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
            _resultAst = _parser.parse(_code1);
            assert(json::parse(_expectedAst) == _resultAst);
        }

        void test4() {
            Parser _parser;
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
                                    "id": {
                                        "type": "ID",
                                        "value": "x"
                                    },
                                    "init": {
                                        "type": "INTEGER",
                                        "value": "24"
                                    },
                                    "type": "VARIABLE_DECLARATION"
                                },
                                {
                                    "id": {
                                        "type": "ID",
                                        "value": "y"
                                    },
                                    "init": {
                                        "type": "FLOAT",
                                        "value": "4.4"
                                    },
                                    "type": "VARIABLE_DECLARATION"
                                },
                                {
                                    "id": {
                                        "type": "ID",
                                        "value": "z"
                                    },
                                    "init": {
                                        "type": "BOOL",
                                        "value": "false"
                                    },
                                    "type": "VARIABLE_DECLARATION"
                                },
                                {
                                    "id": {
                                        "type": "ID",
                                        "value": "s"
                                    },
                                    "init": {
                                        "type": "STRING",
                                        "value": "hello"
                                    },
                                    "type": "VARIABLE_DECLARATION"
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
            _resultAst = _parser.parse(_code1);
            assert(json::parse(_expectedAst) == _resultAst);
        }
};

#endif // __FUNCTIONTESTS_H__