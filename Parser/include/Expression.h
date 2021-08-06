#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include "Parser/include/AssigmentExpression.h"

class Expression {
    private:
        AssigmentExpression assigmentExpression;

    public:
        json getAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
        const AssigmentExpression& getAssigmentExpression() const;
};

#endif // __EXPRESSION_H__