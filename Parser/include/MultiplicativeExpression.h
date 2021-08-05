#ifndef __MULTIPLICATIVEEXPRESSION_H__
#define __MULTIPLICATIVEEXPRESSION_H__

#include "Parser/include/Declarations.h"
#include "Parser/include/RangeExpression.h"

class Expression;
class Parser;

class MultiplicativeExpression {
    private:
        RangeExpression rangeExpression;

    public:
        json getAst(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const;
        const RangeExpression& getRangeExpression() const;
};

#endif // __MULTIPLICATIVEEXPRESSION_H__