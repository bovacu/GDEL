#ifndef __UNARYEXPRESSION_H__
#define __UNARYEXPRESSION_H__

#include "Parser/include/Declarations.h"
#include "Parser/include/PrimaryExpression.h"

class Expression;
class Parser;

class UnaryExpression {
    private:
        PrimaryExpression primaryExpression;

    public:
        json getAst(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const;
        const PrimaryExpression& getPrimaryExpression() const;
};

#endif // __UNARYEXPRESSION_H__