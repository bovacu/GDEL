#include "Parser/include/CallExpression.h"
#include "Parser/include/Defines.h"
#include "Parser/include/Parser.h"
#include "Parser/include/Statement.h"


json CallExpression::getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const {
    auto _member = this->memberExpression.getAst(_statement, _parser, _parser->getLookAhead());
    if(strcmp(_parser->getCurrentLookAheadType().c_str(), _LEFT_PARENTHESIS) == 0)
        return callee(_statement, _parser, _member);

    return _member;
}

const json CallExpression::callee(const Statement& _statement, Parser* _parser, json& _member) const {
    auto _callExpression = json {
        {"type", _CALL_EXPRESSION},
        {"callee", _member},
        {"arguments", getArguments(_statement, _parser, _parser->getLookAhead())}
    };

    if(strcmp(_parser->getCurrentLookAheadType().c_str(), _LEFT_PARENTHESIS) == 0)
        _callExpression = callee(_statement, _parser, _callExpression);

    return _callExpression;
}

const std::vector<json> CallExpression::getArguments(const Statement& _statement, Parser* _parser, json& _member) const {
    _parser->eatToken(_LEFT_PARENTHESIS);
    auto _argsList = strcmp(_parser->getCurrentLookAheadType().c_str(), _RIGHT_PARENTHESIS) == 0 ? std::vector<json>() :
                     getArgumentList(_statement, _parser, _parser->getLookAhead());
    _parser->eatToken(_RIGHT_PARENTHESIS);

    return _argsList;
}

const std::vector<json> CallExpression::getArgumentList(const Statement& _statement, Parser* _parser, json& _member) const {
    std::vector<json> _args;
    
    return _args;
}

const MemberExpression& CallExpression::getMemberExpression() const {
    return this->memberExpression;
}
