#ifndef __LOGICALOREXPRESSION_H__
#define __LOGICALOREXPRESSION_H__

#include "Parser/include/Declarations.h"
#include "Parser/include/LogicalAndExpression.h"

class Statement;
class Parser;

class LogicalOrExpression {
    private:
        LogicalAndExpression logicalAndExpression;

    public:
        json getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const;
        const LogicalAndExpression& getLogicalAndExpression() const;
};

#endif // __LOGICALOREXPRESSION_H__