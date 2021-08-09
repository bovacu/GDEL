#ifndef __IFTESTS_H__
#define __IFTESTS_H__

#include "Parser/include/Parser.hpp"
#include <assert.h>

class IfTests {
    public:
        void runTests() {
            test1();
            test2();
            test3();
            test4();
            std::cout << "\033[1;32m" << "✔" << " All unit test passed for If!" << "\033[0m\n" << std::endl;
        }

    private:
        void test1() {
            const char* _code1 = R"(
                10 + x <= 100 - c;
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
                                        "value": "10"
                                    },
                                    "operator": {
                                        "type": "+",
                                        "value": "+"
                                    },
                                    "right": {
                                        "type": "ID",
                                        "value": "x"
                                    },
                                    "type": "BINARY_OP"
                                },
                                "operator": {
                                    "type": "<=",
                                    "value": "<="
                                },
                                "right": {
                                    "left": {
                                        "type": "INTEGER",
                                        "value": "100"
                                    },
                                    "operator": {
                                        "type": "-",
                                        "value": "-"
                                    },
                                    "right": {
                                        "type": "ID",
                                        "value": "c"
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
            _resultAst = parse(_code1);
            assert(json::parse(_expectedAst) == _resultAst);
            std::cout << "\033[1;32m" << "  ✔" << " If 1!" << "\033[0m\n" << std::endl;
        }

        void test2() {
            const char* _code1 = R"(
                if(x + 3 == y - 3 * 4) {
                    var _result = x + y;
                }
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "alternative": null,
                            "condition": {
                                "left": {
                                    "left": {
                                        "type": "ID",
                                        "value": "x"
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
                                "operator": {
                                    "type": "==",
                                    "value": "=="
                                },
                                "right": {
                                    "left": {
                                        "type": "ID",
                                        "value": "y"
                                    },
                                    "operator": {
                                        "type": "-",
                                        "value": "-"
                                    },
                                    "right": {
                                        "left": {
                                            "type": "INTEGER",
                                            "value": "3"
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
                                    "type": "BINARY_OP"
                                },
                                "type": "BINARY_OP"
                            },
                            "consequence": {
                                "body": [
                                    {
                                        "declarations": [
                                            {
                                                "id": {
                                                    "type": "ID",
                                                    "value": "_result"
                                                },
                                                "init": {
                                                    "left": {
                                                        "type": "ID",
                                                        "value": "x"
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
                                                "type": "VARIABLE_DECLARATION"
                                            }
                                        ],
                                        "type": "VARIABLE_STATEMENT"
                                    }
                                ],
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
            _resultAst = parse(_code1);
            assert(json::parse(_expectedAst) == _resultAst);
            std::cout << "\033[1;32m" << "  ✔" << " If 2!" << "\033[0m\n" << std::endl;
        }

        void test3() {
            const char* _code1 = R"(
                if(a) {
                    # simple comment
                    x = 5 - z;
                    t = true;
                } else {
                    # another comment
                    var _s = 'my string';
                }
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "alternative": {
                                "body": [
                                    {
                                        "declarations": [
                                            {
                                                "id": {
                                                    "type": "ID",
                                                    "value": "_s"
                                                },
                                                "init": {
                                                    "type": "STRING",
                                                    "formattings": [],
                                                    "value": "my string"
                                                },
                                                "type": "VARIABLE_DECLARATION"
                                            }
                                        ],
                                        "type": "VARIABLE_STATEMENT"
                                    }
                                ],
                                "type": "BLOCK_STATEMENT"
                            },
                            "condition": {
                                "type": "ID",
                                "value": "a"
                            },
                            "consequence": {
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
                                                    "value": "5"
                                                },
                                                "operator": {
                                                    "type": "-",
                                                    "value": "-"
                                                },
                                                "right": {
                                                    "type": "ID",
                                                    "value": "z"
                                                },
                                                "type": "BINARY_OP"
                                            },
                                            "type": "ASSIGMENT_EXPRESSION"
                                        },
                                        "type": "EXPRESSION_STATEMENT"
                                    },
                                    {
                                        "expression": {
                                            "left": {
                                                "type": "ID",
                                                "value": "t"
                                            },
                                            "operator": {
                                                "type": "=",
                                                "value": "="
                                            },
                                            "right": {
                                                "type": "BOOL",
                                                "value": "true"
                                            },
                                            "type": "ASSIGMENT_EXPRESSION"
                                        },
                                        "type": "EXPRESSION_STATEMENT"
                                    }
                                ],
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
            _resultAst = parse(_code1);
            assert(json::parse(_expectedAst) == _resultAst);
            std::cout << "\033[1;32m" << "  ✔" << " If 3!" << "\033[0m\n" << std::endl;
        }

        void test4() {
            const char* _code1 = R"(
                if(a > b)
                    b = a;
                else
                    a = b;
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "alternative": {
                                "expression": {
                                    "left": {
                                        "type": "ID",
                                        "value": "a"
                                    },
                                    "operator": {
                                        "type": "=",
                                        "value": "="
                                    },
                                    "right": {
                                        "type": "ID",
                                        "value": "b"
                                    },
                                    "type": "ASSIGMENT_EXPRESSION"
                                },
                                "type": "EXPRESSION_STATEMENT"
                            },
                            "condition": {
                                "left": {
                                    "type": "ID",
                                    "value": "a"
                                },
                                "operator": {
                                    "type": ">",
                                    "value": ">"
                                },
                                "right": {
                                    "type": "ID",
                                    "value": "b"
                                },
                                "type": "BINARY_OP"
                            },
                            "consequence": {
                                "expression": {
                                    "left": {
                                        "type": "ID",
                                        "value": "b"
                                    },
                                    "operator": {
                                        "type": "=",
                                        "value": "="
                                    },
                                    "right": {
                                        "type": "ID",
                                        "value": "a"
                                    },
                                    "type": "ASSIGMENT_EXPRESSION"
                                },
                                "type": "EXPRESSION_STATEMENT"
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
            _resultAst = parse(_code1);
            assert(json::parse(_expectedAst) == _resultAst);
            std::cout << "\033[1;32m" << "  ✔" << " If 4!" << "\033[0m\n" << std::endl;
        }
};

#endif // __IFTESTS_H__