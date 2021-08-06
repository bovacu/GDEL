#include "Parser/include/AssigmentExpression.h"
#include "Parser/include/Parser.h"

json AssigmentExpression::getAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const {
    auto _left = this->logicalOrExpression.getAst(_statement, _parser, _parser.getLookAhead());
    auto _assigment = isAssigment(_parser, _parser.getCurrentLookAheadType().c_str());
    if(_assigment.empty())
        return _left;
    
    if(isCorrectLeftSideAssigment(_parser, _left))
        return json {
            {"type", _ASSIGMENT_EXPRESSION},
            {"operator", _assigment},
            {"left", _left},
            {"right", getAst(_statement, _parser, _parser.getLookAhead())}
        };

    return json {  };
}


const LogicalOrExpression& AssigmentExpression::getLogicalOrExpression() const {
    return this->logicalOrExpression;
}


json AssigmentExpression::isAssigment(Parser& _parser, const char* _tokenType) const {
    // from = to %=
    for(int _i = 13; _i < 21; _i++)
        if(strcmp(symbols[_i], _tokenType) == 0){
            auto _token = _parser.eatToken(symbols[_i]);
            return _token;
        }

    return json {};
}

bool AssigmentExpression::isCorrectLeftSideAssigment(Parser& _parser, json& _tokenToCheck) const {
    if(strcmp(_tokenToCheck["type"].get<std::string>().c_str(), _ID) == 0 ||
    strcmp(_tokenToCheck["type"].get<std::string>().c_str(), _MEMBER_EXPRESSION) == 0) return true;
    throw IllegalAssigmentExpression(_tokenToCheck["type"].get<std::string>().c_str(), _parser.getCurrentParsinLine());
}