#include "Parser/include/Defines.h"
#include "Parser/include/AssigmentExpression.h"
#include "Parser/include/Parser.h"

json AssigmentExpression::getAst(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const {
    auto _left = this->additiveExpression.getAst(_expression, _parser, _parser->getLookAhead());
    auto _assigment = isAssigment(_parser, _parser->getCurrentLookAheadType().c_str());
    if(_assigment.empty())
        return _left;
    
    if(isCorrectLeftSideAssigment(_parser, _left))
        return json {
            {"type", _ASSIGMENT_EXPRESSION},
            {"operator", _assigment},
            {"left", _left},
            {"right", getAst(_expression, _parser, _parser->getLookAhead())}
        };

    return json {  };
}

json AssigmentExpression::isAssigment(Parser* _parser, const char* _tokenType) const {
    // from = to %=
    for(int _i = 13; _i < 21; _i++)
        if(strcmp(symbols[_i], _tokenType) == 0){
            auto _token = _parser->eatToken(symbols[_i]);
            return _token;
        }

    return json {};
}

bool AssigmentExpression::isCorrectLeftSideAssigment(Parser* _parser, json& _tokenToCheck) const {
    if(strcmp(_tokenToCheck["type"].get<std::string>().c_str(), _ID) == 0) return true;
    throw IllegalAssigmentExpression(_tokenToCheck["value"].get<std::string>().c_str(), _parser->getCurrentParsinLine());
}