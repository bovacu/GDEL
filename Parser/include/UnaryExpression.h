#ifndef __UNARYEXPRESSION_H__
#define __UNARYEXPRESSION_H__

#include "Parser/include/Declarations.h"
#include "Parser/include/MemberExpression.h"

class Expression;
class Parser;

class UnaryExpression {
    private:
        MemberExpression memberExpression;

    public:
        json getAst(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const;
        const MemberExpression& getMemberExpression() const;
};

#endif // __UNARYEXPRESSION_H__