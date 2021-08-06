#ifndef __ADDITIVEEXPRESSION_H__
#define __ADDITIVEEXPRESSION_H__

#include "Parser/include/MultiplicativeExpression.h"

class AdditiveExpression {
    private:
        MultiplicativeExpression multiplicativeExpression;

    public:
        json getAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
        const MultiplicativeExpression& getMultiplicativeExpression() const;
};


#endif // __ADDITIVEEXPRESSION_H__