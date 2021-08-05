#include "Parser/include/RelationalExpression.h"
#include "Parser/include/Defines.h"
#include "Parser/include/Parser.h"
#include "Parser/include/Expression.h"


json RelationalExpression::getAst(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const {
    auto _left = this->additiveExpression.getAst(_expression, _parser, _parser->getLookAhead());

    while ( strcmp(_parser->getCurrentLookAheadType().c_str(), _COM_GT) == 0 ||
            strcmp(_parser->getCurrentLookAheadType().c_str(), _COM_GT_EQ) == 0 ||
            strcmp(_parser->getCurrentLookAheadType().c_str(), _COM_LT) == 0 || 
            strcmp(_parser->getCurrentLookAheadType().c_str(), _COM_LT_EQ) == 0) {
        auto _op = _parser->eatToken(_parser->getCurrentLookAheadType().c_str());
        auto _right = this->additiveExpression.getAst(_expression, _parser, _parser->getLookAhead());
        _left = {
            {"type", _BINARY_OP},
            {"operator", _op},
            {"left", _left},
            {"right", _right}
        };
    }

    return _left;
}


const AdditiveExpression& RelationalExpression::getAdditiveExpression() const {
    return this->additiveExpression;
}

