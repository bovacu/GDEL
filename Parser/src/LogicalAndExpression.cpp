#include "Parser/include/LogicalAndExpression.h"
#include "Parser/include/Parser.h"

json LogicalAndExpression::getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const {
    auto _left = this->equalityExpression.getAst(_statement, _parser, _parser->getLookAhead());
    
    while ( strcmp(_parser->getCurrentLookAheadType().c_str(), _COM_AND) == 0) {
        auto _op = _parser->eatToken(_parser->getCurrentLookAheadType().c_str());
        auto _right = this->equalityExpression.getAst(_statement, _parser, _parser->getLookAhead());
        _left = {
            {"type", _LOGICAL_EXPRESSION},
            {"operator", _op},
            {"left", _left},
            {"right", _right}
        };
    }

    return _left;
}


const EqualityExpression& LogicalAndExpression::getEqualityExpression() const {
    return this->equalityExpression;
}
