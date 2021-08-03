#include "ExpressionStatement.h"
#include "Parser/core/Parser.h"

json ExpressionStatement::getNextToken(Parser* _parser, const char* _code, int& _charPointer, int _linePointer) {
    return this->expression.getNextToken(_parser, _code, _charPointer, _linePointer);
}

json ExpressionStatement::getAst(Parser* _parser, json& _tokenToCheck) const {
    auto _ast = this->expression.getAst(_parser, _tokenToCheck);
    _parser->eatToken(_SEMICOLON);
    return json {
        {"type", _EXPRESSION_STATEMENT},
        {"expression", _ast}
    };
}
