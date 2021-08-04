#ifndef __PRIMARYEXPRESSION_H__
#define __PRIMARYEXPRESSION_H__

#include "Parser/include/Declarations.h"
#include "Parser/include/ParenthesisExpression.h"
#include "Parser/include/Literal.h"

class Expression;
class Parser;

class PrimaryExpression {
    private:
        ParenthesisExpression parenthesisExpression;
        Literal literal;

    public:
        json getAst(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const;
};

#endif // __PRIMARYEXPRESSION_H__