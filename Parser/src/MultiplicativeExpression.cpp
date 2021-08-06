#include "Parser/include/MultiplicativeExpression.h"
#include "Parser/include/Parser.h"

json MultiplicativeExpression::getAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const {
    auto _left = this->rangeExpression.getAst(_statement, _parser, _parser.getLookAhead());

    while ( strcmp(_parser.getCurrentLookAheadType().c_str(), _MUL) == 0 || 
            strcmp(_parser.getCurrentLookAheadType().c_str(), _DIV) == 0) {
        auto _op = _parser.eatToken(_parser.getCurrentLookAheadType().c_str());
        auto _right = this->rangeExpression.getAst(_statement, _parser, _parser.getLookAhead());
        _left = {
            {"type", _BINARY_OP},
            {"operator", _op},
            {"left", _left},
            {"right", _right}
        };
    }

    return _left;
}

const RangeExpression& MultiplicativeExpression::getRangeExpression() const{
    return this->rangeExpression;
}  

