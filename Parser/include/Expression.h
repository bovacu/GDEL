#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include "Parser/include/Declarations.h"
#include "Parser/include/AdditiveExpression.h"
#include "Parser/include/Expression.h"

class Parser;

class Expression {
    private:
        AdditiveExpression additiveExpression;

    public:
        json getAst(Parser* _parser, json& _tokenToCheck) const;
};

#endif // __EXPRESSION_H__