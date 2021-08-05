#ifndef __LOGICALANDEXPRESSION_H__
#define __LOGICALANDEXPRESSION_H__

#include "Parser/include/Declarations.h"
#include "Parser/include/EqualityExpression.h"

class Expression;
class Parser;

class LogicalAndExpression {
    private:
        EqualityExpression equalityExpression;

    public:
        json getAst(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const;
        const EqualityExpression& getEqualityExpression() const;
};

#endif // __LOGICALANDEXPRESSION_H__