#include "Parser/include/ExpressionStatement.h"
#include "Parser/include/Parser.h"

json ExpressionStatement::getAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const {
    auto _ast = this->expression.getAst(_statement, _parser, _tokenToCheck);
    _parser.eatToken(_SEMICOLON);
    return json {
        {"type", _EXPRESSION_STATEMENT},
        {"expression", _ast}
    };
}


const Expression& ExpressionStatement::getExpression() const {
    return this->expression;
}

