#ifndef __RANGEEXPRESSION_H__
#define __RANGEEXPRESSION_H__

#include "Parser/include/Declarations.h"
#include "Parser/include/UnaryExpression.h"

class Expression;
class Parser;

class RangeExpression {
    private:
        UnaryExpression unaryExpression;
    
    public:
        json getAst(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const;
        const UnaryExpression& getUnaryExpression() const;
};

#endif // __RANGEEXPRESSION_H__