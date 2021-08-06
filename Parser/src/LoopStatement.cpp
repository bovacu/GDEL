#include "Parser/include/LoopStatement.h"
#include "Parser/include/Parser.h"

json LoopStatement::getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const {
    const Expression& _expression = _statement.getExpressionStatement().getExpression();
    _parser->eatToken(_LOOP);
    _parser->eatToken(_LEFT_PARENTHESIS);
    auto _condition = _expression.getAst(_statement, _parser, _parser->getLookAhead());
    _parser->eatToken(_RIGHT_PARENTHESIS);
    auto _loopBody = _statement.getAst(_parser, _parser->getLookAhead());

    return {
        {"type", _LOOP_STATEMENT},
        {"condition", _condition},
        {"loopBody", _loopBody}
    };
}