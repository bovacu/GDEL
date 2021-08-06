#include "Parser/include/EqualityExpression.h"
#include "Parser/include/Defines.h"
#include "Parser/include/Parser.h"
#include "Parser/include/Statement.h"


json EqualityExpression::getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const {
    auto _left = this->relationalExpression.getAst(_statement, _parser, _parser->getLookAhead());

    while ( strcmp(_parser->getCurrentLookAheadType().c_str(), _COM_EQ) == 0 ||
            strcmp(_parser->getCurrentLookAheadType().c_str(), _COM_N_EQ) == 0) {
        auto _op = _parser->eatToken(_parser->getCurrentLookAheadType().c_str());
        auto _right = this->relationalExpression.getAst(_statement, _parser, _parser->getLookAhead());
        _left = {
            {"type", _BINARY_OP},
            {"operator", _op},
            {"left", _left},
            {"right", _right}
        };
    }

    return _left;
}


const RelationalExpression& EqualityExpression::getRelationalExpression() const {
    return this->relationalExpression;
}
