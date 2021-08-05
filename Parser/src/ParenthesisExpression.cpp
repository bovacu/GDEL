#include "Parser/include/Defines.h"
#include "Parser/include/ParenthesisExpression.h"
#include "Parser/include/Expression.h"
#include "Parser/include/Parser.h"

json ParenthesisExpression::getAst(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const {
    _parser->eatToken(_LEFT_PARENTHESIS);
    auto _token = _expression.getAst(_parser, _parser->getLookAhead());
    _parser->eatToken(_RIGHT_PARENTHESIS);
    return _token;
}