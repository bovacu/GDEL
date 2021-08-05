#include "Parser/include/ForLoopStatement.h"
#include "Parser/include/Defines.h"
#include "Parser/include/Parser.h"
#include "Parser/include/Expression.h"
#include "Parser/include/Statement.h"

json ForLoopStatement::getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const {
    const Expression& _expression = _statement.getExpressionStatement().getExpression();
    _parser->eatToken(_FOR);
    _parser->eatToken(_LEFT_PARENTHESIS);
    auto _condition = _expression.getAst(_parser, _parser->getLookAhead());

    _parser->eatToken(_VAR);
    auto _pivotVar = _parser->eatToken(_ID);

    const char* _loopStatement = _FOR_STATEMENT;
    if(strcmp(_parser->getCurrentLookAheadType().c_str(), _IN) == 0) {
        _parser->eatToken(_IN);
        auto _iterableVar = _statement.getAst(_parser, _parser->getLookAhead());
        _loopStatement = _FOREACH_STATEMENT;
    } else {

    }

    _parser->eatToken(_RIGHT_PARENTHESIS);
    auto _loopBody = _statement.getAst(_parser, _parser->getLookAhead());

    return {
        {"type", _FOREACH_STATEMENT},
        {"condition", _condition},
        {"consequence", _loopBody}
    };
}