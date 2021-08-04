#ifndef __MULTIPLICATIVEEXPRESSION_H__
#define __MULTIPLICATIVEEXPRESSION_H__

#include "Parser/include/Declarations.h"
#include "Parser/include/PrimaryExpression.h"

class Expression;
class Parser;

class MultiplicativeExpression {
    private:
        PrimaryExpression primaryExpression;

    public:
        json getAst(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const;
};

#endif // __MULTIPLICATIVEEXPRESSION_H__