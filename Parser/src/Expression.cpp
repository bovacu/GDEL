#include "Parser/include/Defines.h"
#include "Parser/include/Expression.h"
#include "Parser/include/Parser.h"

json Expression::getAst(Parser* _parser, json& _tokenToCheck) const {
    return this->assigmentExpression.getAst(*this, _parser, _tokenToCheck);
}
