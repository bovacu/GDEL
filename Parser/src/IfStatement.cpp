#include "Parser/include/IfStatement.h"
#include "Parser/include/Parser.h"

json IfStatement::getAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const {
    const Expression& _expression = _statement.getExpressionStatement().getExpression();
    _parser.eatToken(_IF);
    _parser.eatToken(_LEFT_PARENTHESIS);
    auto _condition = _expression.getAst(_statement, _parser, _parser.getLookAhead());
    _parser.eatToken(_RIGHT_PARENTHESIS);
    auto _consequence = _statement.getAst(_parser, _parser.getLookAhead());

    // std::vector<json> _alternatives;
    // if(strcmp(_parser->getCurrentLookAheadType().c_str(), _ELSE_IF) == 0) {
    //     _parser->eatToken(_ELSE_IF);
    //     _parser->eatToken(_LEFT_PARENTHESIS);
    //     auto _alternativeCondition = _expression.getAst(_parser, _parser->getLookAhead());
    //     _parser->eatToken(_RIGHT_PARENTHESIS);
    //     auto _alternativeConsequence = _statement.getAst(_parser, _parser->getLookAhead());
    //     json _elseif =  {
    //         {"type", _ELSE_IF_STATEMENT},
    //         {"condition", _alternativeCondition},
    //         {"consequence", _alternativeConsequence},
    //     };
    //     _alternatives.push_back(_elseif);
    // }

    json _alternative = {};
    if(!_parser.getLookAhead().empty() && strcmp(_parser.getCurrentLookAheadType().c_str(), _ELSE) == 0) {
        _parser.eatToken(_ELSE);
        _alternative = _statement.getAst(_parser, _parser.getLookAhead());
    }

    return {
        {"type", _IF_STATEMENT},
        {"condition", _condition},
        {"consequence", _consequence},
        {"alternative", _alternative}
    };
}
