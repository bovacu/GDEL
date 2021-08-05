#ifndef __ASSIGMENTTESTS_H__
#define __ASSIGMENTTESTS_H__

#include "Parser/include/Parser.h"
#include "Parser/include/Declarations.h"
#include <assert.h>

class AssigmentTests {
        public:
        void runTests() {
            test1();
            test2();
            test3();
            test4();
            test5();
            test6();
            std::cout << "\033[1;32m" << "✔" << " All unit test passed for Assigment!" << "\033[0m\n" << std::endl;
        }

    private:
        void test1() {
            Parser _parser;
            const char* _code1 = R"(
                x = 0;
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "expression": {
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
                                    "value": "0"
                                },
                                "type": "ASSIGMENT_EXPRESSION"
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
                x = 4 + 5;
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "expression": {
                                "left": {
                                    "type": "ID",
                                    "value": "x"
                                },
                                "operator": {
                                    "type": "=",
                                    "value": "="
                                },
                                "right": {
                                    "left": {
                                        "type": "INTEGER",
                                        "value": "4"
                                    },
                                    "operator": {
                                        "type": "+",
                                        "value": "+"
                                    },
                                    "right": {
                                        "type": "INTEGER",
                                        "value": "5"
                                    },
                                    "type": "BINARY_OP"
                                },
                                "type": "ASSIGMENT_EXPRESSION"
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
                x = (4 + 5) * 4.55;
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "expression": {
                                "left": {
                                    "type": "ID",
                                    "value": "x"
                                },
                                "operator": {
                                    "type": "=",
                                    "value": "="
                                },
                                "right": {
                                    "left": {
                                        "left": {
                                            "type": "INTEGER",
                                            "value": "4"
                                        },
                                        "operator": {
                                            "type": "+",
                                            "value": "+"
                                        },
                                        "right": {
                                            "type": "INTEGER",
                                            "value": "5"
                                        },
                                        "type": "BINARY_OP"
                                    },
                                    "operator": {
                                        "type": "*",
                                        "value": "*"
                                    },
                                    "right": {
                                        "type": "FLOAT",
                                        "value": "4.55"
                                    },
                                    "type": "BINARY_OP"
                                },
                                "type": "ASSIGMENT_EXPRESSION"
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
                x = y = (4 + 5) / (1 + 3);
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "expression": {
                                "left": {
                                    "type": "ID",
                                    "value": "x"
                                },
                                "operator": {
                                    "type": "=",
                                    "value": "="
                                },
                                "right": {
                                    "left": {
                                        "type": "ID",
                                        "value": "y"
                                    },
                                    "operator": {
                                        "type": "=",
                                        "value": "="
                                    },
                                    "right": {
                                        "left": {
                                            "left": {
                                                "type": "INTEGER",
                                                "value": "4"
                                            },
                                            "operator": {
                                                "type": "+",
                                                "value": "+"
                                            },
                                            "right": {
                                                "type": "INTEGER",
                                                "value": "5"
                                            },
                                            "type": "BINARY_OP"
                                        },
                                        "operator": {
                                            "type": "/",
                                            "value": "/"
                                        },
                                        "right": {
                                            "left": {
                                                "type": "INTEGER",
                                                "value": "1"
                                            },
                                            "operator": {
                                                "type": "+",
                                                "value": "+"
                                            },
                                            "right": {
                                                "type": "INTEGER",
                                                "value": "3"
                                            },
                                            "type": "BINARY_OP"
                                        },
                                        "type": "BINARY_OP"
                                    },
                                    "type": "ASSIGMENT_EXPRESSION"
                                },
                                "type": "ASSIGMENT_EXPRESSION"
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
                x = 4 + 5 + y;
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "expression": {
                                "left": {
                                    "type": "ID",
                                    "value": "x"
                                },
                                "operator": {
                                    "type": "=",
                                    "value": "="
                                },
                                "right": {
                                    "left": {
                                        "left": {
                                            "type": "INTEGER",
                                            "value": "4"
                                        },
                                        "operator": {
                                            "type": "+",
                                            "value": "+"
                                        },
                                        "right": {
                                            "type": "INTEGER",
                                            "value": "5"
                                        },
                                        "type": "BINARY_OP"
                                    },
                                    "operator": {
                                        "type": "+",
                                        "value": "+"
                                    },
                                    "right": {
                                        "type": "ID",
                                        "value": "y"
                                    },
                                    "type": "BINARY_OP"
                                },
                                "type": "ASSIGMENT_EXPRESSION"
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
                x += 1;
                x -= 1;
                x *= 1;
                x /= 1;
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "expression": {
                                "left": {
                                    "type": "ID",
                                    "value": "x"
                                },
                                "operator": {
                                    "type": "+=",
                                    "value": "+="
                                },
                                "right": {
                                    "type": "INTEGER",
                                    "value": "1"
                                },
                                "type": "ASSIGMENT_EXPRESSION"
                            },
                            "type": "EXPRESSION_STATEMENT"
                        },
                        {
                            "expression": {
                                "left": {
                                    "type": "ID",
                                    "value": "x"
                                },
                                "operator": {
                                    "type": "-=",
                                    "value": "-="
                                },
                                "right": {
                                    "type": "INTEGER",
                                    "value": "1"
                                },
                                "type": "ASSIGMENT_EXPRESSION"
                            },
                            "type": "EXPRESSION_STATEMENT"
                        },
                        {
                            "expression": {
                                "left": {
                                    "type": "ID",
                                    "value": "x"
                                },
                                "operator": {
                                    "type": "*=",
                                    "value": "*="
                                },
                                "right": {
                                    "type": "INTEGER",
                                    "value": "1"
                                },
                                "type": "ASSIGMENT_EXPRESSION"
                            },
                            "type": "EXPRESSION_STATEMENT"
                        },
                        {
                            "expression": {
                                "left": {
                                    "type": "ID",
                                    "value": "x"
                                },
                                "operator": {
                                    "type": "/=",
                                    "value": "/="
                                },
                                "right": {
                                    "type": "INTEGER",
                                    "value": "1"
                                },
                                "type": "ASSIGMENT_EXPRESSION"
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

#endif // __ASSIGMENTTESTS_H__