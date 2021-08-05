#include "Parser/include/RangeExpression.h"
#include "Parser/include/Defines.h"
#include "Parser/include/Parser.h"
#include "Parser/include/Expression.h"


json RangeExpression::getAst(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const {
    auto _left = this->unaryExpression.getAst(_expression, _parser, _parser->getLookAhead());
    
    while ( strcmp(_parser->getCurrentLookAheadType().c_str(), _INCL_RANGE) == 0 ||
            strcmp(_parser->getCurrentLookAheadType().c_str(), _EXCL_RANGE) == 0) {
        auto _op = _parser->eatToken(_parser->getCurrentLookAheadType().c_str());
        auto _right = this->unaryExpression.getAst(_expression, _parser, _parser->getLookAhead());
        _left = {
            {"type", _RANGE},
            {"operator", _op},
            {"left", _left},
            {"right", _right}
        };
    }

    return _left;
}


const UnaryExpression& RangeExpression::getUnaryExpression() const {
    return this->unaryExpression;
}
