#ifndef __PRIMARYEXPRESSION_H__
#define __PRIMARYEXPRESSION_H__

#include "Parser/include/ParenthesisExpression.h"
#include "Parser/include/Literal.h"

class PrimaryExpression {
    private:
        ParenthesisExpression parenthesisExpression;
        Literal literal;

    public:
        json getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const;
        const Literal& getLiteral() const;
};

#endif // __PRIMARYEXPRESSION_H__