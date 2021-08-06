#ifndef __EXPRESSIONSTATEMENT_H__
#define __EXPRESSIONSTATEMENT_H__

#include "Parser/include/Expression.h"

class ExpressionStatement {
    private:
        Expression expression;

    public:
        json getAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
        const Expression& getExpression() const;
};

#endif // __EXPRESSIONSTATEMENT_H__