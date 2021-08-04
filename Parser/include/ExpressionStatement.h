#ifndef __EXPRESSIONSTATEMENT_H__
#define __EXPRESSIONSTATEMENT_H__

#include "Parser/include/Declarations.h"
#include "Parser/include/Expression.h"

class Parser;

class ExpressionStatement {
    private:
        Expression expression;

    public:
        json getAst(Parser* _parser, json& _tokenToCheck) const;
};

#endif // __EXPRESSIONSTATEMENT_H__