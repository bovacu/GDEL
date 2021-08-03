#include "Expression.h"
#include "Parser/core/Parser.h"

json Expression::getNextToken(Parser* _parser, const char* _code, int& _charPointer, int _linePointer) {
    return this->literal.getNextToken(_parser, _code, _charPointer, _linePointer);
}

json Expression::getAst(Parser* _parser, json& _tokenToCheck) const {
    return this->literal.getAst(_parser, _tokenToCheck);
}
