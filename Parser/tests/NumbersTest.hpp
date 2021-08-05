#ifndef __NUMBERSTEST_H__
#define __NUMBERSTEST_H__

#include "Parser/include/Parser.h"
#include "Parser/include/Defines.h"
#include "Parser/include/Declarations.h"
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
            Parser _parser;
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

            _resultAst = _parser.parse(_code1);
            assert(_resultAst == _expectedAst);
        }

        void numbers2() {
            Parser _parser;
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

            _resultAst = _parser.parse(_code1);
            assert(_resultAst == _expectedAst);
        }

        void numbers3() {
            Parser _parser;
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

            _resultAst = _parser.parse(_code1);
            assert(_resultAst == _expectedAst);
        }

        void numbersFailParsingBySemicolon() {
            Parser _parser;
            const char* _code1 = R"(
                123
            )";
            json _resultAst;
            json _expectedAst = {
                {"type", _PROGRAM},
                {"body", std::vector<json> {
                        {
                            {"expression", {
                                    {"type", _INTEGER},
                                    {"value", "123"}
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

            try {
                _resultAst = _parser.parse(_code1);
                assert(false);
            } catch(const std::exception& e) {
                assert(true);
            }
        }
};

#endif // __NUMBERSTEST_H__