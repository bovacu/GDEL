#ifndef __EQUALITYEXPRESSION_H__
#define __EQUALITYEXPRESSION_H__

#include "Parser/include/Declarations.h"
#include "Parser/include/RelationalExpression.h"

class Parser;

class EqualityExpression {
    private:
        RelationalExpression relationalExpression;
    
    public:
        json getAst(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const;
        const RelationalExpression& getRelationalExpression() const;
};

#endif // __EQUALITYEXPRESSION_H__