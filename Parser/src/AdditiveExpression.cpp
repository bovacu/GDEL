#include "Parser/include/AdditiveExpression.h"
#include "Parser/include/Parser.h"

json AdditiveExpression::getAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const {
    auto _left = this->multiplicativeExpression.getAst(_statement, _parser, _parser.getLookAhead());
    
    while ( strcmp(_parser.getCurrentLookAheadType().c_str(), _SUM) == 0 || 
            strcmp(_parser.getCurrentLookAheadType().c_str(), _SUB) == 0) {
        auto _op = _parser.eatToken(_parser.getCurrentLookAheadType().c_str());
        auto _right = this->multiplicativeExpression.getAst(_statement, _parser, _parser.getLookAhead());
        _left = {
            {"type", _BINARY_OP},
            {"operator", _op},
            {"left", _left},
            {"right", _right}
        };
    }

    return _left;
}



const MultiplicativeExpression& AdditiveExpression::getMultiplicativeExpression() const {
    return this->multiplicativeExpression;
}

