#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include "Parser/core/Declarations.h"
#include "Parser/elements/literal/Literal.h"

class Parser;

class Expression {
    private:
        Literal literal;

    public:
        json getNextToken(Parser* _parser, const char* _code, int& _charPointer, int _linePointer);
        json getAst(Parser* _parser, json& _tokenToCheck) const;
};

#endif // __EXPRESSION_H__