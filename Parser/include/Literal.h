#ifndef LITERAL_H
#define LITERAL_H

#include "Parser/include/Declarations.h"
class Parser;

class Literal {
    public:
        json getNextToken(Parser* _parser, const char* _code, int& _charPointer, int _line);
        json getAst(Parser* _parser, json& _tokenToCheck) const;

    private:
        void numberToken(Parser* _parser, json& _outToken, const char* _code, int& _charPointer, int _line);
        void stringToken(Parser* _parser, json& _outToken, const char* _code, int& _charPointer, int _line);
        void boolToken(Parser* _parser, json& _outToken, const char* _code, int& _charPointer, int _line);
        void symbolsToken(Parser* _parser, json& _outToken, const char* _code, int& _charPointer, int _line);

        json numberAst(Parser* _parser, json& _tokenToCheck) const;
        json stringAst(Parser* _parser, json& _tokenToCheck) const;
        json boolAst(Parser* _parser, json& _tokenToCheck) const;
        json symbolAst(Parser* _parser, json& _tokenToCheck) const;

    private:
        const char* symbols[5] = {_SPACE, _SEMICOLON, _N, _R, _T};
};

#endif //LITERAL_H