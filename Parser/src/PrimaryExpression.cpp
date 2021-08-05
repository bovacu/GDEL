#include "Parser/include/Defines.h"
#include "Parser/include/PrimaryExpression.h"
#include "Parser/include/Parser.h"

json PrimaryExpression::getAst(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const {
    if(strcmp(_tokenToCheck["type"].get<std::string>().c_str(), _LEFT_PARENTHESIS) == 0)
        return this->parenthesisExpression.getAst(_expression, _parser, _parser->getLookAhead());
    else
        return this->literal.getAst(_parser, _parser->getLookAhead());
}