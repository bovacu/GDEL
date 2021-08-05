#ifndef __LOOPTESTS_H__
#define __LOOPTESTS_H__

#include "Parser/include/Parser.h"
#include "Parser/include/Declarations.h"
#include <assert.h>

class LoopTests {
    public:
        void runTests() {
            test1();
            test2();
            test3();
            test4();
            std::cout << "\033[1;32m" << "✔" << " All unit test passed for Loops!" << "\033[0m\n" << std::endl;
        }

    private:
        void test1() {
            Parser _parser;
            const char* _code1 = R"(
                for(var _z = 0 in 8..9) {
                       
                }
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "index": [
                                {
                                    "id": {
                                        "type": "ID",
                                        "value": "_z"
                                    },
                                    "init": {
                                        "type": "INTEGER",
                                        "value": "0"
                                    },
                                    "type": "VARIABLE_DECLARATION"
                                }
                            ],
                            "loopBody": {
                                "body": [],
                                "type": "BLOCK_STATEMENT"
                            },
                            "range": {
                                "left": {
                                    "type": "INTEGER",
                                    "value": "8"
                                },
                                "operator": {
                                    "type": "..",
                                    "value": ".."
                                },
                                "right": {
                                    "type": "INTEGER",
                                    "value": "9"
                                },
                                "type": "RANGE"
                            },
                            "type": "FOR_STATEMENT"
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
                for(var _i in x) {
                   var _y = _i + 1;
               }
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "index": [
                                {
                                    "id": {
                                        "type": "ID",
                                        "value": "_i"
                                    },
                                    "init": null,
                                    "type": "VARIABLE_DECLARATION"
                                }
                            ],
                            "loopBody": {
                                "body": [
                                    {
                                        "declarations": [
                                            {
                                                "id": {
                                                    "type": "ID",
                                                    "value": "_y"
                                                },
                                                "init": {
                                                    "left": {
                                                        "type": "ID",
                                                        "value": "_i"
                                                    },
                                                    "operator": {
                                                        "type": "+",
                                                        "value": "+"
                                                    },
                                                    "right": {
                                                        "type": "INTEGER",
                                                        "value": "1"
                                                    },
                                                    "type": "BINARY_OP"
                                                },
                                                "type": "VARIABLE_DECLARATION"
                                            }
                                        ],
                                        "type": "VARIABLE_STATEMENT"
                                    }
                                ],
                                "type": "BLOCK_STATEMENT"
                            },
                            "range": {
                                "type": "ID",
                                "value": "x"
                            },
                            "type": "FOR_STATEMENT"
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
                loop(x) {
                   
                }
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "condition": {
                                "type": "ID",
                                "value": "x"
                            },
                            "loopBody": {
                                "body": [],
                                "type": "BLOCK_STATEMENT"
                            },
                            "type": "LOOP_STATEMENT"
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
                loop(x > 5) {
                    if(x == 3) {
                        for(var _z in 9...10) {
                            var _string = 'stringyyy';
                        }
                    }
                }
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "condition": {
                                "left": {
                                    "type": "ID",
                                    "value": "x"
                                },
                                "operator": {
                                    "type": ">",
                                    "value": ">"
                                },
                                "right": {
                                    "type": "INTEGER",
                                    "value": "5"
                                },
                                "type": "BINARY_OP"
                            },
                            "loopBody": {
                                "body": [
                                    {
                                        "alternative": null,
                                        "condition": {
                                            "left": {
                                                "type": "ID",
                                                "value": "x"
                                            },
                                            "operator": {
                                                "type": "==",
                                                "value": "=="
                                            },
                                            "right": {
                                                "type": "INTEGER",
                                                "value": "3"
                                            },
                                            "type": "BINARY_OP"
                                        },
                                        "consequence": {
                                            "body": [
                                                {
                                                    "index": [
                                                        {
                                                            "id": {
                                                                "type": "ID",
                                                                "value": "_z"
                                                            },
                                                            "init": null,
                                                            "type": "VARIABLE_DECLARATION"
                                                        }
                                                    ],
                                                    "loopBody": {
                                                        "body": [
                                                            {
                                                                "declarations": [
                                                                    {
                                                                        "id": {
                                                                            "type": "ID",
                                                                            "value": "_string"
                                                                        },
                                                                        "init": {
                                                                            "type": "STRING",
                                                                            "value": "stringyyy"
                                                                        },
                                                                        "type": "VARIABLE_DECLARATION"
                                                                    }
                                                                ],
                                                                "type": "VARIABLE_STATEMENT"
                                                            }
                                                        ],
                                                        "type": "BLOCK_STATEMENT"
                                                    },
                                                    "range": {
                                                        "left": {
                                                            "type": "INTEGER",
                                                            "value": "9"
                                                        },
                                                        "operator": {
                                                            "type": "...",
                                                            "value": "..."
                                                        },
                                                        "right": {
                                                            "type": "INTEGER",
                                                            "value": "10"
                                                        },
                                                        "type": "RANGE"
                                                    },
                                                    "type": "FOR_STATEMENT"
                                                }
                                            ],
                                            "type": "BLOCK_STATEMENT"
                                        },
                                        "type": "IF_STATEMENT"
                                    }
                                ],
                                "type": "BLOCK_STATEMENT"
                            },
                            "type": "LOOP_STATEMENT"
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

#endif // __LOOPTESTS_H__