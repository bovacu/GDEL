#ifndef __EXPRESSIONSTATEMENT_H__
#define __EXPRESSIONSTATEMENT_H__

#include "Parser/core/Declarations.h"
#include "Parser/elements/statement/expression/Expression.h"

class Parser;

class ExpressionStatement {
    private:
        Expression expression;

    public:
        json getNextToken(Parser* _parser, const char* _code, int& _charPointer, int _linePointer);
        json getAst(Parser* _parser, json& _tokenToCheck) const;
};

#endif // __EXPRESSIONSTATEMENT_H__