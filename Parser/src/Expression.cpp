#include "Parser/include/Defines.h"
#include "Parser/include/Expression.h"
#include "Parser/include/Parser.h"
#include "Parser/include/Statement.h"

json Expression::getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const {
    return this->assigmentExpression.getAst(_statement, _parser, _tokenToCheck);
}


const AssigmentExpression& Expression::getAssigmentExpression() const {
    return this->assigmentExpression;
}
