#ifndef __EQUALITYEXPRESSION_H__
#define __EQUALITYEXPRESSION_H__

#include "Parser/include/RelationalExpression.h"

class EqualityExpression {
    private:
        RelationalExpression relationalExpression;
    
    public:
        json getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const;
        const RelationalExpression& getRelationalExpression() const;
};

#endif // __EQUALITYEXPRESSION_H__