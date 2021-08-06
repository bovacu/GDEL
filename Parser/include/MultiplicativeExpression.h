#ifndef __MULTIPLICATIVEEXPRESSION_H__
#define __MULTIPLICATIVEEXPRESSION_H__

#include "Parser/include/RangeExpression.h"

class MultiplicativeExpression {
    private:
        RangeExpression rangeExpression;

    public:
        json getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const;
        const RangeExpression& getRangeExpression() const;
};

#endif // __MULTIPLICATIVEEXPRESSION_H__