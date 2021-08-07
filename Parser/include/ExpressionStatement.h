#ifndef __EXPRESSIONSTATEMENT_H__
#define __EXPRESSIONSTATEMENT_H__

#include "Parser/include/IStatement.h"
#include "Parser/include/Expression.h"

class ExpressionStatement : public IStatement{
    private:
        Expression expression;

    public:
        json getAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
        const Expression& getExpression() const;
        inline Func getFunc() override { return BIND_FN(ExpressionStatement::getAst); }
};

#endif // __EXPRESSIONSTATEMENT_H__