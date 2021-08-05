#include "Parser/include/Defines.h"
#include "Parser/include/AdditiveExpression.h"
#include "Parser/include/Parser.h"
#include "Parser/include/Expression.h"

json AdditiveExpression::getAst(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const {
    auto _left = this->multiplicativeExpression.getAst(_expression, _parser, _parser->getLookAhead());
    
    while ( strcmp(_parser->getCurrentLookAheadType().c_str(), _SUM) == 0 || 
            strcmp(_parser->getCurrentLookAheadType().c_str(), _SUB) == 0) {
        auto _op = _parser->eatToken(_parser->getCurrentLookAheadType().c_str());
        auto _right = this->multiplicativeExpression.getAst(_expression, _parser, _parser->getLookAhead());
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

