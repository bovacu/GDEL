#ifndef __RELATIONALEXPRESSION_H__
#define __RELATIONALEXPRESSION_H__

#include "Parser/include/Declarations.h"
#include "Parser/include/AdditiveExpression.h"
class Parser;
class Statement;

class RelationalExpression {
    private:
        AdditiveExpression additiveExpression;
    public:
        json getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const;
        const AdditiveExpression& getAdditiveExpression() const;
};

#endif // __RELATIONALEXPRESSION_H__