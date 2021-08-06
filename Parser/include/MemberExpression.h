#ifndef __MEMBEREXPRESSION_H__
#define __MEMBEREXPRESSION_H__

#include "Parser/include/PrimaryExpression.h"

class MemberExpression {
    private:
        PrimaryExpression primaryExpression;

    public:
        json getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const;
        const PrimaryExpression& getPrimaryExpression() const;
};

#endif // __MEMBEREXPRESSION_H__