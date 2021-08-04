#ifndef __PARENTHESISEXPRESSION_H__
#define __PARENTHESISEXPRESSION_H__

#include "Parser/include/Declarations.h"

class Expression;
class Parser;

class ParenthesisExpression {
    public:
        json getAst(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const;
};


#endif // __PARENTHESISEXPRESSION_H__