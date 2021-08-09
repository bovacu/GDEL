#ifndef __VARIABLESTEST_H__
#define __VARIABLESTEST_H__

#include "Parser/include/Parser.hpp"
#include <assert.h>

class VariablesTest {
        public:
        void runTests() {
            test1();
            test2();
            test3();
            test4();
            std::cout << "\033[1;32m" << "✔" << " All unit test passed for Variables!" << "\033[0m\n" << std::endl;
        }

    private:
        void test1() {
            const char* _code1 = R"(
                var x;
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
                                        "value": "x"
                                    },
                                    "init": null,
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
            std::cout << "\033[1;32m" << "✔" << " Variables 1!" << "\033[0m\n" << std::endl;
        }

        void test2() {
            const char* _code1 = R"(
                var x, y, z, c, t;
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
                                        "value": "x"
                                    },
                                    "init": null,
                                    "type": "VARIABLE_DECLARATION"
                                },
                                {
                                    "id": {
                                        "type": "ID",
                                        "value": "y"
                                    },
                                    "init": null,
                                    "type": "VARIABLE_DECLARATION"
                                },
                                {
                                    "id": {
                                        "type": "ID",
                                        "value": "z"
                                    },
                                    "init": null,
                                    "type": "VARIABLE_DECLARATION"
                                },
                                {
                                    "id": {
                                        "type": "ID",
                                        "value": "c"
                                    },
                                    "init": null,
                                    "type": "VARIABLE_DECLARATION"
                                },
                                {
                                    "id": {
                                        "type": "ID",
                                        "value": "t"
                                    },
                                    "init": null,
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
            std::cout << "\033[1;32m" << "✔" << " Variables 2!" << "\033[0m\n" << std::endl;
        }

        void test3() {
            const char* _code1 = R"(
                var x, y = (5 + 5) * 3, z, c = w, t;
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
                                        "value": "x"
                                    },
                                    "init": null,
                                    "type": "VARIABLE_DECLARATION"
                                },
                                {
                                    "id": {
                                        "type": "ID",
                                        "value": "y"
                                    },
                                    "init": {
                                        "left": {
                                            "left": {
                                                "type": "INTEGER",
                                                "value": "5"
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
                                            "type": "INTEGER",
                                            "value": "3"
                                        },
                                        "type": "BINARY_OP"
                                    },
                                    "type": "VARIABLE_DECLARATION"
                                },
                                {
                                    "id": {
                                        "type": "ID",
                                        "value": "z"
                                    },
                                    "init": null,
                                    "type": "VARIABLE_DECLARATION"
                                },
                                {
                                    "id": {
                                        "type": "ID",
                                        "value": "c"
                                    },
                                    "init": {
                                        "type": "ID",
                                        "value": "w"
                                    },
                                    "type": "VARIABLE_DECLARATION"
                                },
                                {
                                    "id": {
                                        "type": "ID",
                                        "value": "t"
                                    },
                                    "init": null,
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
            std::cout << "\033[1;32m" << "✔" << " Variables 3!" << "\033[0m\n" << std::endl;
        }

        void test4() {
            const char* _code1 = R"(
                var x = 0;
                var _y = 4;
                var _myVar;
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
                                        "value": "x"
                                    },
                                    "init": {
                                        "type": "INTEGER",
                                        "value": "0"
                                    },
                                    "type": "VARIABLE_DECLARATION"
                                }
                            ],
                            "type": "VARIABLE_STATEMENT"
                        },
                        {
                            "declarations": [
                                {
                                    "id": {
                                        "type": "ID",
                                        "value": "_y"
                                    },
                                    "init": {
                                        "type": "INTEGER",
                                        "value": "4"
                                    },
                                    "type": "VARIABLE_DECLARATION"
                                }
                            ],
                            "type": "VARIABLE_STATEMENT"
                        },
                        {
                            "declarations": [
                                {
                                    "id": {
                                        "type": "ID",
                                        "value": "_myVar"
                                    },
                                    "init": null,
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
            std::cout << "\033[1;32m" << "✔" << " Variables 4!" << "\033[0m\n" << std::endl;
        }
};

#endif // __VARIABLESTEST_H__