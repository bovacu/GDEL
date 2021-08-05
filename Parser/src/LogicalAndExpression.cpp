#include "Parser/include/LogicalAndExpression.h"
#include "Parser/include/Defines.h"
#include "Parser/include/Parser.h"
#include "Parser/include/Expression.h"

json LogicalAndExpression::getAst(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const {
    auto _left = this->equalityExpression.getAst(_expression, _parser, _parser->getLookAhead());
    
    while ( strcmp(_parser->getCurrentLookAheadType().c_str(), _COM_AND) == 0) {
        auto _op = _parser->eatToken(_parser->getCurrentLookAheadType().c_str());
        auto _right = this->equalityExpression.getAst(_expression, _parser, _parser->getLookAhead());
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
