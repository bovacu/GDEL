#ifndef __ASSIGMENTEXPRESSION_H__
#define __ASSIGMENTEXPRESSION_H__

#include "Parser/include/Declarations.h"
#include "Parser/include/LogicalOrExpression.h"

class Expression;
class Parser;

class AssigmentExpression {
    private:
        LogicalOrExpression logicalOrExpression;

    public:
        json getAst(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const;
        const LogicalOrExpression& getLogicalOrExpression() const;

    private:
        json isAssigment(Parser* _parser, const char* _tokenType) const;
        bool isCorrectLeftSideAssigment(Parser* _parser, json& _tokenToCheck) const;
};

#endif // __ASSIGMENTEXPRESSION_H__