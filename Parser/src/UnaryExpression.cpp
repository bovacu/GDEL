#include "Parser/include/UnaryExpression.h"
#include "Parser/include/Parser.h"

json UnaryExpression::getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const {
    json _unaryOperator; // ! or - or +
    if(strcmp(_parser->getCurrentLookAheadType().c_str(), _SUM) == 0 || 
       strcmp(_parser->getCurrentLookAheadType().c_str(), _SUB) == 0 ||
       strcmp(_parser->getCurrentLookAheadType().c_str(), _COM_N) == 0) {
           _unaryOperator = _parser->eatToken(_parser->getCurrentLookAheadType().c_str());
       }

    if(!_unaryOperator.empty()) {
        return json {
            {"type", _UNARY_OP},
            {"operator", _unaryOperator},
            {"argument", getAst(_statement, _parser, _parser->getLookAhead())}
        };
    }

    return this->callExpression.getAst(_statement, _parser, _parser->getLookAhead());
}

const CallExpression& UnaryExpression::getCallExpression() const {
    return this->callExpression;
}
