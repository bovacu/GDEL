#ifndef __STRUCTTESTS_H__
#define __STRUCTTESTS_H__

#include "Parser/include/Parser.h"
#include "Parser/include/Declarations.h"
#include <assert.h>

class StructTests {
    public:
        void runTests() {
            test1();
            std::cout << "\033[1;32m" << "✔" << " All unit test passed for Struct!" << "\033[0m\n" << std::endl;
        }

    private:
        void test1() {
            Parser _parser;
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
                }

                var _myPoint = new Point(10, 5);
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
                                                "id": {
                                                    "type": "ID",
                                                    "value": "_x"
                                                },
                                                "init": null,
                                                "type": "VARIABLE_DECLARATION"
                                            },
                                            {
                                                "id": {
                                                    "type": "ID",
                                                    "value": "_y"
                                                },
                                                "init": null,
                                                "type": "VARIABLE_DECLARATION"
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
                                                "id": {
                                                    "type": "INTEGER",
                                                    "value": "10"
                                                },
                                                "init": null,
                                                "type": "VARIABLE_DECLARATION"
                                            },
                                            {
                                                "id": {
                                                    "type": "INTEGER",
                                                    "value": "5"
                                                },
                                                "init": null,
                                                "type": "VARIABLE_DECLARATION"
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
            _resultAst = _parser.parse(_code1);
            assert(json::parse(_expectedAst) == _resultAst);
        }
};

#endif // __STRUCTTESTS_H__