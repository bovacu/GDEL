#ifndef __LOGICALANDEXPRESSION_H__
#define __LOGICALANDEXPRESSION_H__

#include "Parser/include/EqualityExpression.h"

class LogicalAndExpression {
    private:
        EqualityExpression equalityExpression;

    public:
        json getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const;
        const EqualityExpression& getEqualityExpression() const;
};

#endif // __LOGICALANDEXPRESSION_H__