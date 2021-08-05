#ifndef __ADDITIVEEXPRESSION_H__
#define __ADDITIVEEXPRESSION_H__

#include "Parser/include/Declarations.h"
#include "Parser/include/MultiplicativeExpression.h"

class Expression;
class Parser;

class AdditiveExpression {
    private:
        MultiplicativeExpression multiplicativeExpression;

    public:
        json getAst(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const;
        const MultiplicativeExpression& getMultiplicativeExpression() const;
};


#endif // __ADDITIVEEXPRESSION_H__