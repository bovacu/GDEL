#ifndef __MULTIPLICATIVEEXPRESSION_H__
#define __MULTIPLICATIVEEXPRESSION_H__

#include "Parser/include/Declarations.h"
#include "Parser/include/UnaryExpression.h"

class Expression;
class Parser;

class MultiplicativeExpression {
    private:
        UnaryExpression unaryExpression;

    public:
        json getAst(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const;
        const UnaryExpression& getUnaryExpression() const;
};

#endif // __MULTIPLICATIVEEXPRESSION_H__