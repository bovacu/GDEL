#include "Parser/include/PrimaryExpression.h"
#include "Parser/include/Parser.h"

json PrimaryExpression::getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const {
    if(strcmp(_tokenToCheck["type"].get<std::string>().c_str(), _LEFT_PARENTHESIS) == 0)
        return this->parenthesisExpression.getAst(_statement, _parser, _parser->getLookAhead());
    else
        return this->literal.getAst(_statement, _parser, _parser->getLookAhead());
}