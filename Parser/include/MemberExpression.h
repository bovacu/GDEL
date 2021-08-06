#ifndef __MEMBEREXPRESSION_H__
#define __MEMBEREXPRESSION_H__

#include "Parser/include/Declarations.h"
#include "Parser/include/PrimaryExpression.h"
class Expression;

class MemberExpression {
    private:
        PrimaryExpression primaryExpression;

    public:
        json getAst(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const;
        const PrimaryExpression& getPrimaryExpression() const;
};

#endif // __MEMBEREXPRESSION_H__