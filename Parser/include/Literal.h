#ifndef LITERAL_H
#define LITERAL_H

#include "Parser/include/Declarations.h"
class Parser;

class Literal {
    public:
        json getAst(Parser* _parser, json& _tokenToCheck) const;

    private:
        json numberAst(Parser* _parser, json& _tokenToCheck) const;
        json stringAst(Parser* _parser, json& _tokenToCheck) const;
        json boolAst(Parser* _parser, json& _tokenToCheck) const;
        json symbolAst(Parser* _parser, json& _tokenToCheck) const;

    private:
        const char* symbols[15] = { _SPACE, _SEMICOLON, _N, _R, _T, _SUM, _DIV, _MUL, 
                                    _SUB, _MOD, _POW, _LEFT_PARENTHESIS, _RIGHT_PARENTHESIS};
};

#endif //LITERAL_H