#ifndef __ASSIGMENTEXPRESSION_H__
#define __ASSIGMENTEXPRESSION_H__

#include "Parser/include/Declarations.h"
#include "Parser/include/AdditiveExpression.h"

class Expression;
class Parser;

class AssigmentExpression {
    private:
        AdditiveExpression additiveExpression;

    public:
        json getAst(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const;

    private:
        json isAssigment(Parser* _parser, const char* _tokenType) const;
        bool isCorrectLeftSideAssigment(Parser* _parser, json& _tokenToCheck) const;
};

#endif // __ASSIGMENTEXPRESSION_H__