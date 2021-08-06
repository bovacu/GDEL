#ifndef __UNARYEXPRESSION_H__
#define __UNARYEXPRESSION_H__

#include "Parser/include/CallExpression.h"

class UnaryExpression {
    private:
        CallExpression callExpression;

    public:
        json getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const;
        const CallExpression& getCallExpression() const;
};

#endif // __UNARYEXPRESSION_H__