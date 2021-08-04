#ifndef __BINARYOPSTEST_H__
#define __BINARYOPSTEST_H__

#include "Parser/include/Parser.h"
#include "Parser/include/Declarations.h"
#include <assert.h>


class BinaryOpsTest {
    public:
        void runTests() {
            test1();
            test2();
            test3();
            test4();
            test5();
            test6();
            std::cout << "\033[1;32m" << "✔" << " All unit test passed for BinaryOps!" << "\033[0m\n" << std::endl;
        }

    private:
        void test1() {
            Parser _parser;
            const char* _code1 = R"(
                45 + 33;
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "expression": {
                                "left": {
                                    "type": "INTEGER",
                                    "value": "45"
                                },
                                "operator": {
                                    "type": "+",
                                    "value": "+"
                                },
                                "right": {
                                    "type": "INTEGER",
                                    "value": "33"
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

        void test2() {
            Parser _parser;
            const char* _code1 = R"(
                45 + 33 - 95;
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "expression": {
                                "left": {
                                    "left": {
                                        "type": "INTEGER",
                                        "value": "45"
                                    },
                                    "operator": {
                                        "type": "+",
                                        "value": "+"
                                    },
                                    "right": {
                                        "type": "INTEGER",
                                        "value": "33"
                                    },
                                    "type": "BINARY_OP"
                                },
                                "operator": {
                                    "type": "-",
                                    "value": "-"
                                },
                                "right": {
                                    "type": "INTEGER",
                                    "value": "95"
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

        void test3() {
            Parser _parser;
            const char* _code1 = R"(
                3 + 2 * 95;
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "expression": {
                                "left": {
                                    "type": "INTEGER",
                                    "value": "3"
                                },
                                "operator": {
                                    "type": "+",
                                    "value": "+"
                                },
                                "right": {
                                    "left": {
                                        "type": "INTEGER",
                                        "value": "2"
                                    },
                                    "operator": {
                                        "type": "*",
                                        "value": "*"
                                    },
                                    "right": {
                                        "type": "INTEGER",
                                        "value": "95"
                                    },
                                    "type": "BINARY_OP"
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

        void test4() {
            Parser _parser;
            const char* _code1 = R"(
                3 / 2 - 95;
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "expression": {
                                "left": {
                                    "left": {
                                        "type": "INTEGER",
                                        "value": "3"
                                    },
                                    "operator": {
                                        "type": "/",
                                        "value": "/"
                                    },
                                    "right": {
                                        "type": "INTEGER",
                                        "value": "2"
                                    },
                                    "type": "BINARY_OP"
                                },
                                "operator": {
                                    "type": "-",
                                    "value": "-"
                                },
                                "right": {
                                    "type": "INTEGER",
                                    "value": "95"
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

        void test5() {
            Parser _parser;
            const char* _code1 = R"(
                3 + 2 * 95 / 5 - 1;
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "expression": {
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
                                        "left": {
                                            "left": {
                                                "type": "INTEGER",
                                                "value": "2"
                                            },
                                            "operator": {
                                                "type": "*",
                                                "value": "*"
                                            },
                                            "right": {
                                                "type": "INTEGER",
                                                "value": "95"
                                            },
                                            "type": "BINARY_OP"
                                        },
                                        "operator": {
                                            "type": "/",
                                            "value": "/"
                                        },
                                        "right": {
                                            "type": "INTEGER",
                                            "value": "5"
                                        },
                                        "type": "BINARY_OP"
                                    },
                                    "type": "BINARY_OP"
                                },
                                "operator": {
                                    "type": "-",
                                    "value": "-"
                                },
                                "right": {
                                    "type": "INTEGER",
                                    "value": "1"
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

        void test6() {
            Parser _parser;
            const char* _code1 = R"(
                (3 + 2) * 4;
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "expression": {
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
                                        "value": "2"
                                    },
                                    "type": "BINARY_OP"
                                },
                                "operator": {
                                    "type": "*",
                                    "value": "*"
                                },
                                "right": {
                                    "type": "INTEGER",
                                    "value": "4"
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

#endif // __BINARYOPSTEST_H__