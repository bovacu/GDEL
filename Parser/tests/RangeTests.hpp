#ifndef __RANGETESTS_H__
#define __RANGETESTS_H__

#include "Parser/include/Parser.h"
#include "Parser/include/Declarations.h"
#include <assert.h>

class RangeTests {
    public:
        void runTests() {
            test1();
            test2();
            test3();
            test4();
            test5();
            std::cout << "\033[1;32m" << "✔" << " All unit test passed for Range!" << "\033[0m\n" << std::endl;
        }

    private:
        void test1() {
            Parser _parser;
            const char* _code1 = R"(
                8..9;
                10...12;
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "expression": {
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
                            "type": "EXPRESSION_STATEMENT"
                        },
                        {
                            "expression": {
                                "left": {
                                    "type": "INTEGER",
                                    "value": "10"
                                },
                                "operator": {
                                    "type": "...",
                                    "value": "..."
                                },
                                "right": {
                                    "type": "INTEGER",
                                    "value": "12"
                                },
                                "type": "RANGE"
                            },
                            "type": "EXPRESSION_STATEMENT"
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
                -8...-9;
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "expression": {
                                "left": {
                                    "argument": {
                                        "type": "INTEGER",
                                        "value": "8"
                                    },
                                    "operator": {
                                        "type": "-",
                                        "value": "-"
                                    },
                                    "type": "UNARY_OP"
                                },
                                "operator": {
                                    "type": "...",
                                    "value": "..."
                                },
                                "right": {
                                    "argument": {
                                        "type": "INTEGER",
                                        "value": "9"
                                    },
                                    "operator": {
                                        "type": "-",
                                        "value": "-"
                                    },
                                    "type": "UNARY_OP"
                                },
                                "type": "RANGE"
                            },
                            "type": "EXPRESSION_STATEMENT"
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
                var _r = (3+1)..5 + 7;
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "declarations": [
                                {
                                    "id": {
                                        "type": "ID",
                                        "value": "_r"
                                    },
                                    "init": {
                                        "left": {
                                            "left": {
                                                "left": {
                                                    "type": "INTEGER",
                                                    "value": "3"
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
                                            "operator": {
                                                "type": "..",
                                                "value": ".."
                                            },
                                            "right": {
                                                "type": "INTEGER",
                                                "value": "5"
                                            },
                                            "type": "RANGE"
                                        },
                                        "operator": {
                                            "type": "+",
                                            "value": "+"
                                        },
                                        "right": {
                                            "type": "INTEGER",
                                            "value": "7"
                                        },
                                        "type": "BINARY_OP"
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
            _resultAst = _parser.parse(_code1);
            assert(json::parse(_expectedAst) == _resultAst);
        }

        void test4() {
            Parser _parser;
            const char* _code1 = R"(
                if(8...10) {  }
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "alternative": null,
                            "condition": {
                                "left": {
                                    "type": "INTEGER",
                                    "value": "8"
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
                            "consequence": {
                                "body": [],
                                "type": "BLOCK_STATEMENT"
                            },
                            "type": "IF_STATEMENT"
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

        void test5() {
            Parser _parser;
            const char* _code1 = R"(
                -8...10 + 9..12;
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "expression": {
                                "left": {
                                    "left": {
                                        "argument": {
                                            "type": "INTEGER",
                                            "value": "8"
                                        },
                                        "operator": {
                                            "type": "-",
                                            "value": "-"
                                        },
                                        "type": "UNARY_OP"
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
                                "operator": {
                                    "type": "+",
                                    "value": "+"
                                },
                                "right": {
                                    "left": {
                                        "type": "INTEGER",
                                        "value": "9"
                                    },
                                    "operator": {
                                        "type": "..",
                                        "value": ".."
                                    },
                                    "right": {
                                        "type": "INTEGER",
                                        "value": "12"
                                    },
                                    "type": "RANGE"
                                },
                                "type": "BINARY_OP"
                            },
                            "type": "EXPRESSION_STATEMENT"
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

#endif // __RANGETESTS_H__