#include "Parser/include/ForLoopStatement.h"
#include "Parser/include/Parser.h"

json ForLoopStatement::getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const {
    const Expression& _expression = _statement.getExpressionStatement().getExpression();
    const VariableStatement& _variableStatement = _statement.getVariableStatement();
    _parser->eatToken(_FOR);
    _parser->eatToken(_LEFT_PARENTHESIS);

    _parser->eatToken(_VAR);
    std::vector<json> _indexVar = _variableStatement.getDeclarationList(_statement, _statement.getExpressionStatement().getExpression(), _parser, _parser->getLookAhead(), true);
    _parser->eatToken(_IN);
    auto _rangeVar = _expression.getAst(_statement, _parser, _parser->getLookAhead());

    _parser->eatToken(_RIGHT_PARENTHESIS);
    auto _loopBody = _statement.getAst(_parser, _parser->getLookAhead());

    return {
        {"type", _FOR_STATEMENT},
        {"index", _indexVar},
        {"range", _rangeVar},
        {"loopBody", _loopBody}
    };
}