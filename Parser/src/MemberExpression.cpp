#include "Parser/include/MemberExpression.h"
#include "Parser/include/Parser.h"

json MemberExpression::getAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const {
    auto _object = this->primaryExpression.getAst(_statement, _parser, _parser.getLookAhead());

    while(strcmp(_parser.getCurrentLookAheadType().c_str(), _DOT) == 0 || 
          strcmp(_parser.getCurrentLookAheadType().c_str(), _LEFT_BRACKET) == 0) {
        
        if(strcmp(_parser.getCurrentLookAheadType().c_str(), _DOT) == 0) {
            _parser.eatToken(_DOT);
            auto _prop = _parser.eatToken(_ID);
            _object = {
                {"type", _MEMBER_EXPRESSION},
                {"computed", false},
                {"object", _object},
                {"property", _prop}
            };
        } else {
            _parser.eatToken(_LEFT_BRACKET);
            auto _prop = _statement.getExpressionStatement().getExpression().getAst(_statement, _parser, _parser.getLookAhead());
            _parser.eatToken(_RIGHT_BRACKET);
            _object = {
                {"type", _MEMBER_EXPRESSION},
                {"computed", true},
                {"object", _object},
                {"property", _prop}
            };
        }
    }

    return _object;
}


const PrimaryExpression& MemberExpression::getPrimaryExpression() const {
    return this->primaryExpression;
}
