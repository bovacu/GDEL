#include "Parser/include/Defines.h"
#include "Parser/include/Statement.h"
#include "Parser/include/Parser.h"

json Statement::getAst(Parser* _parser, json& _tokenToCheck) const {
    auto _tokenType = _tokenToCheck["type"].get<std::string>();

    if(strcmp(_tokenType.c_str(), _LEFT_COLLIBRACE) == 0) {
        return this->blockStatement.getAst(*this, _parser, _parser->getLookAhead());
    } else if(strcmp(_tokenType.c_str(), _VAR) == 0) {
        return this->variableStatement.getAst(this->expressionStatement.getExpression(), _parser, _parser->getLookAhead());
    } else if (strcmp(_tokenType.c_str(), _IF) == 0) {
        return this->ifStatement.getAst(*this, _parser, _parser->getLookAhead());
    } else if (strcmp(_tokenType.c_str(), _LOOP) == 0) {
        return this->loopStatement.getAst(*this, _parser, _parser->getLookAhead());
    }

    return this->expressionStatement.getAst(_parser, _tokenToCheck);
}


const ExpressionStatement& Statement::getExpressionStatement() const {
    return this->expressionStatement;
}

