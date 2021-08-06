#include "Parser/include/RangeExpression.h"
#include "Parser/include/Parser.h"

json RangeExpression::getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const {
    auto _left = this->unaryExpression.getAst(_statement, _parser, _parser->getLookAhead());
    
    while ( strcmp(_parser->getCurrentLookAheadType().c_str(), _INCL_RANGE) == 0 ||
            strcmp(_parser->getCurrentLookAheadType().c_str(), _EXCL_RANGE) == 0) {
        auto _op = _parser->eatToken(_parser->getCurrentLookAheadType().c_str());
        auto _right = this->unaryExpression.getAst(_statement, _parser, _parser->getLookAhead());
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
