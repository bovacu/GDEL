#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include "Parser/include/Declarations.h"
#include "Parser/include/AssigmentExpression.h"

class Parser;

class Expression {
    private:
        AssigmentExpression assigmentExpression;

    public:
        json getAst(Parser* _parser, json& _tokenToCheck) const;
        const AssigmentExpression& getAssigmentExpression() const;
};

#endif // __EXPRESSION_H__