#ifndef __NUMBERSTEST_H__
#define __NUMBERSTEST_H__

#include "Parser/include/Parser.hpp"
#include <assert.h>

class NumbersTest {
    public:
        void runTests() {
            runNumbers();
            std::cout << "\033[1;32m" << "✔" << " All unit test passed for NumberTests!" << "\033[0m\n" << std::endl;
        }

    private:
        void runNumbers() {
            numbers1();
            numbers2();
            numbers3();
        }

        void numbers1() {
            const char* _code1 = R"(45;)";
            json _resultAst;
            
            json _expectedAst = {
                {"type", _PROGRAM},
                {"body", std::vector<json> {
                        {
                            {"expression", {
                                    {"type", _INTEGER},
                                    {"value", "45"}
                                }
                            },
                            {"type", _EXPRESSION_STATEMENT}
                        },
                        {
                            {"type", _EOF},
                            {"value", _EOF}
                        }
                    }
                }
            };

            _resultAst = parse(_code1);
            assert(_resultAst == _expectedAst);
            std::cout << "\033[1;32m" << "  ✔" << " NumberTests 1!" << "\033[0m\n" << std::endl;
        }

        void numbers2() {
            const char* _code1 = R"(
                36578;
            )";
            json _resultAst;
            json _expectedAst = {
                {"type", _PROGRAM},
                {"body", std::vector<json> {
                        {
                            {"expression", {
                                    {"type", _INTEGER},
                                    {"value", "36578"}
                                }
                            },
                            {"type", _EXPRESSION_STATEMENT}
                        },
                        {
                            {"type", _EOF},
                            {"value", _EOF}
                        }
                    }
                }
            };

            _resultAst = parse(_code1);
            assert(_resultAst == _expectedAst);
            std::cout << "\033[1;32m" << "  ✔" << " NumberTests 2!" << "\033[0m\n" << std::endl;
        }

        void numbers3() {
            const char* _code1 = R"(
                36578;
                128.256;
                256;
            )";
            json _resultAst;
            json _expectedAst = {
                {"type", _PROGRAM},
                {"body", std::vector<json> {
                        {
                            {"expression", {
                                    {"type", _INTEGER},
                                    {"value", "36578"}
                                }
                            },
                            {"type", _EXPRESSION_STATEMENT}
                        },
                        {
                            {"expression", {
                                    {"type", _FLOAT},
                                    {"value", "128.256"}
                                }
                            },
                            {"type", _EXPRESSION_STATEMENT}
                        },
                        {
                            {"expression", {
                                    {"type", _INTEGER},
                                    {"value", "256"}
                                }
                            },
                            {"type", _EXPRESSION_STATEMENT}
                        },
                        {
                            {"type", _EOF},
                            {"value", _EOF}
                        }
                    }
                }
            };

            _resultAst = parse(_code1);
            assert(_resultAst == _expectedAst);
            std::cout << "\033[1;32m" << "  ✔" << " NumberTests 3!" << "\033[0m\n" << std::endl;
        }
};

#endif // __NUMBERSTEST_H__