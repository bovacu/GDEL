#include "Parser/include/ParenthesisExpression.h"
#include "Parser/include/Parser.h"

json ParenthesisExpression::getAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const {
    _parser.eatToken(_LEFT_PARENTHESIS);
    auto _token = _statement.getExpressionStatement().getExpression().getAst(_statement, _parser, _parser.getLookAhead());
    _parser.eatToken(_RIGHT_PARENTHESIS);
    return _token;
}