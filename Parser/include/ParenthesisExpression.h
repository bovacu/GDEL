#ifndef __PARENTHESISEXPRESSION_H__
#define __PARENTHESISEXPRESSION_H__

#include "Parser/include/Declarations.h"

class Statement;
class Parser;

class ParenthesisExpression {
    public:
        json getAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
};


#endif // __PARENTHESISEXPRESSION_H__