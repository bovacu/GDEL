#ifndef __STRUCTTESTS_H__
#define __STRUCTTESTS_H__

#include "Parser/include/Parser.hpp"
#include <assert.h>

class StructTests {
    public:
        void runTests() {
            test1();
            std::cout << "\033[1;32m" << "✔" << " All unit test passed for Struct!" << "\033[0m\n" << std::endl;
        }

    private:
        void test1() {
            const char* _code1 = R"(
                struct Point extends BasePoint {
                    func constructor(_x, _y) {
                        this.x = _x;
                        this.y = _y;
                    }

                    func getX() {
                        ret this.x;
                    }

                    func getY() {
                        ret this.y;
                    }

                    func print() {
                        console.log(self.x, self.y, getX(), getY(), z[0]);
                    }
                }

                var _myPoint = new Point(10, y[1]);
            )";
            json _resultAst;
            
            std::string _expectedAst = R"(
                {
                    "body": [
                        {
                            "extends": {
                                "type": "ID",
                                "value": "BasePoint"
                            },
                            "name": {
                                "type": "ID",
                                "value": "Point"
                            },
                            "structBody": {
                                "body": [
                                    {
                                        "body": {
                                            "body": [
                                                {
                                                    "expression": {
                                                        "left": {
                                                            "computed": false,
                                                            "object": {
                                                                "type": "ID",
                                                                "value": "this"
                                                            },
                                                            "property": {
                                                                "type": "ID",
                                                                "value": "x"
                                                            },
                                                            "type": "MEMBER_EXPRESSION"
                                                        },
                                                        "operator": {
                                                            "type": "=",
                                                            "value": "="
                                                        },
                                                        "right": {
                                                            "type": "ID",
                                                            "value": "_x"
                                                        },
                                                        "type": "ASSIGMENT_EXPRESSION"
                                                    },
                                                    "type": "EXPRESSION_STATEMENT"
                                                },
                                                {
                                                    "expression": {
                                                        "left": {
                                                            "computed": false,
                                                            "object": {
                                                                "type": "ID",
                                                                "value": "this"
                                                            },
                                                            "property": {
                                                                "type": "ID",
                                                                "value": "y"
                                                            },
                                                            "type": "MEMBER_EXPRESSION"
                                                        },
                                                        "operator": {
                                                            "type": "=",
                                                            "value": "="
                                                        },
                                                        "right": {
                                                            "type": "ID",
                                                            "value": "_y"
                                                        },
                                                        "type": "ASSIGMENT_EXPRESSION"
                                                    },
                                                    "type": "EXPRESSION_STATEMENT"
                                                }
                                            ],
                                            "type": "BLOCK_STATEMENT"
                                        },
                                        "name": {
                                            "type": "ID",
                                            "value": "constructor"
                                        },
                                        "params": [
                                            {
                                                "type": "ID",
                                                "value": "_x"
                                            },
                                            {
                                                "type": "ID",
                                                "value": "_y"
                                            }
                                        ],
                                        "type": "FUNCTION_DECLARATION"
                                    },
                                    {
                                        "body": {
                                            "body": [
                                                {
                                                    "arguments": {
                                                        "computed": false,
                                                        "object": {
                                                            "type": "ID",
                                                            "value": "this"
                                                        },
                                                        "property": {
                                                            "type": "ID",
                                                            "value": "x"
                                                        },
                                                        "type": "MEMBER_EXPRESSION"
                                                    },
                                                    "type": "RET_STATEMENT"
                                                }
                                            ],
                                            "type": "BLOCK_STATEMENT"
                                        },
                                        "name": {
                                            "type": "ID",
                                            "value": "getX"
                                        },
                                        "params": [],
                                        "type": "FUNCTION_DECLARATION"
                                    },
                                    {
                                        "body": {
                                            "body": [
                                                {
                                                    "arguments": {
                                                        "computed": false,
                                                        "object": {
                                                            "type": "ID",
                                                            "value": "this"
                                                        },
                                                        "property": {
                                                            "type": "ID",
                                                            "value": "y"
                                                        },
                                                        "type": "MEMBER_EXPRESSION"
                                                    },
                                                    "type": "RET_STATEMENT"
                                                }
                                            ],
                                            "type": "BLOCK_STATEMENT"
                                        },
                                        "name": {
                                            "type": "ID",
                                            "value": "getY"
                                        },
                                        "params": [],
                                        "type": "FUNCTION_DECLARATION"
                                    },
                                    {
                                        "body": {
                                            "body": [
                                                {
                                                    "expression": {
                                                        "arguments": [
                                                            {
                                                                "computed": false,
                                                                "object": {
                                                                    "type": "SEFL_EXPRESSION"
                                                                },
                                                                "property": {
                                                                    "type": "ID",
                                                                    "value": "x"
                                                                },
                                                                "type": "MEMBER_EXPRESSION"
                                                            },
                                                            {
                                                                "computed": false,
                                                                "object": {
                                                                    "type": "SEFL_EXPRESSION"
                                                                },
                                                                "property": {
                                                                    "type": "ID",
                                                                    "value": "y"
                                                                },
                                                                "type": "MEMBER_EXPRESSION"
                                                            },
                                                            {
                                                                "arguments": [],
                                                                "callee": {
                                                                    "type": "ID",
                                                                    "value": "getX"
                                                                },
                                                                "type": "CALL_EXPRESSION"
                                                            },
                                                            {
                                                                "arguments": [],
                                                                "callee": {
                                                                    "type": "ID",
                                                                    "value": "getY"
                                                                },
                                                                "type": "CALL_EXPRESSION"
                                                            },
                                                            {
                                                                "computed": true,
                                                                "object": {
                                                                    "type": "ID",
                                                                    "value": "z"
                                                                },
                                                                "property": {
                                                                    "type": "INTEGER",
                                                                    "value": "0"
                                                                },
                                                                "type": "MEMBER_EXPRESSION"
                                                            }
                                                        ],
                                                        "callee": {
                                                            "computed": false,
                                                            "object": {
                                                                "type": "ID",
                                                                "value": "console"
                                                            },
                                                            "property": {
                                                                "type": "ID",
                                                                "value": "log"
                                                            },
                                                            "type": "MEMBER_EXPRESSION"
                                                        },
                                                        "type": "CALL_EXPRESSION"
                                                    },
                                                    "type": "EXPRESSION_STATEMENT"
                                                }
                                            ],
                                            "type": "BLOCK_STATEMENT"
                                        },
                                        "name": {
                                            "type": "ID",
                                            "value": "print"
                                        },
                                        "params": [],
                                        "type": "FUNCTION_DECLARATION"
                                    }
                                ],
                                "type": "BLOCK_STATEMENT"
                            },
                            "type": "STRUCT_STATEMENT"
                        },
                        {
                            "declarations": [
                                {
                                    "id": {
                                        "type": "ID",
                                        "value": "_myPoint"
                                    },
                                    "init": {
                                        "args": [
                                            {
                                                "type": "INTEGER",
                                                "value": "10"
                                            },
                                            {
                                                "computed": true,
                                                "object": {
                                                    "type": "ID",
                                                    "value": "y"
                                                },
                                                "property": {
                                                    "type": "INTEGER",
                                                    "value": "1"
                                                },
                                                "type": "MEMBER_EXPRESSION"
                                            }
                                        ],
                                        "callee": {
                                            "type": "ID",
                                            "value": "Point"
                                        },
                                        "type": "NEW_EXPRESSION"
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
            std::cout << "\033[1;32m" << "  ✔" << " Struct 1!" << "\033[0m\n" << std::endl;
        }
};

#endif // __STRUCTTESTS_H__