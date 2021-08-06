#include "Parser/include/FunctionDeclaration.h"
#include "Parser/include/Defines.h"
#include "Parser/include/Parser.h"
#include "Parser/include/Statement.h"

json FunctionDeclaration::getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const {
    _parser->eatToken(_FUNC);
    auto _funcName = _parser->eatToken(_ID);
    _parser->eatToken(_LEFT_PARENTHESIS);
    auto _params = getParamList(_statement, _parser, _parser->getLookAhead());
    _parser->eatToken(_RIGHT_PARENTHESIS);
    auto _block = _statement.getBlockStatement().getAst(_statement, _parser, _parser->getLookAhead());

    return json {
        {"type", _FUNC_DECL},
        {"name", _funcName},
        {"params", _params},
        {"body", _block}
    };
}


json FunctionDeclaration::getReturnAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const {
    _parser->eatToken(_RET);
    auto _ret = strcmp(_parser->getCurrentLookAheadType().c_str(), _SEMICOLON) == 0 ? json {  } : 
        _statement.getExpressionStatement().getExpression().getAst(_parser, _parser->getLookAhead());
    _parser->eatToken(_SEMICOLON);
    
    return json {
        {"type", _RET_STATEMENT},
        {"arguments", _ret}
    };
}


std::vector<json> FunctionDeclaration::getParamList(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const {
    if(strcmp(_parser->getCurrentLookAheadType().c_str(), _RIGHT_PARENTHESIS) == 0) return std::vector<json>();
    auto _expression = _statement.getExpressionStatement().getExpression();
    return _statement.getVariableStatement().getDeclarationList(_expression, _parser, _parser->getLookAhead(), true);
}
