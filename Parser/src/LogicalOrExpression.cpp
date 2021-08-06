#include "Parser/include/LogicalOrExpression.h"
#include "Parser/include/Parser.h"

json LogicalOrExpression::getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const {
    auto _left = this->logicalAndExpression.getAst(_statement, _parser, _parser->getLookAhead());
    
    while ( strcmp(_parser->getCurrentLookAheadType().c_str(), _COM_OR) == 0) {
        auto _op = _parser->eatToken(_parser->getCurrentLookAheadType().c_str());
        auto _right = this->logicalAndExpression.getAst(_statement, _parser, _parser->getLookAhead());
        _left = {
            {"type", _LOGICAL_EXPRESSION},
            {"operator", _op},
            {"left", _left},
            {"right", _right}
        };
    }

    return _left;
}


const LogicalAndExpression& LogicalOrExpression::getLogicalAndExpression() const {
    return this->logicalAndExpression;
}