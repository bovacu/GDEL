#include "Parser/include/Statement.h"
#include "Parser/include/Parser.h"

json Statement::getAst(Parser& _parser, json& _tokenToCheck) const {
    auto _tokenType = _tokenToCheck["type"].get<std::string>();

    if(strcmp(_tokenType.c_str(), _LEFT_COLLIBRACE) == 0) {
        return this->blockStatement.getAst(*this, _parser, _parser.getLookAhead());
    } else if(strcmp(_tokenType.c_str(), _VAR) == 0) {
        return this->variableStatement.getAst(*this, _parser, _parser.getLookAhead());
    } else if (strcmp(_tokenType.c_str(), _IF) == 0) {
        return this->ifStatement.getAst(*this, _parser, _parser.getLookAhead());
    } else if (strcmp(_tokenType.c_str(), _LOOP) == 0) {
        return this->loopStatement.getAst(*this, _parser, _parser.getLookAhead());
    } else if (strcmp(_tokenType.c_str(), _FOR) == 0) {
        return this->forLoopStatement.getAst(*this, _parser, _parser.getLookAhead());
    } else if(strcmp(_tokenType.c_str(), _FUNC) == 0) {
        return this->functionDeclaration.getAst(*this, _parser, _parser.getLookAhead());
    } else if(strcmp(_tokenType.c_str(), _RET) == 0) {
        return this->functionDeclaration.getReturnAst(*this, _parser, _parser.getLookAhead());
    }

    return this->expressionStatement.getAst(*this, _parser, _tokenToCheck);
}


const ExpressionStatement& Statement::getExpressionStatement() const {
    return this->expressionStatement;
}

const BlockStatement& Statement::getBlockStatement() const {
    return this->blockStatement;
}

const VariableStatement& Statement::getVariableStatement() const {
    return this->variableStatement;
}

const IfStatement& Statement::getIfStatement() const {
    return this->ifStatement;
}

const LoopStatement& Statement::getLoopStatement() const {
    return this->loopStatement;
}

const ForLoopStatement& Statement::getForLoopStatement() const {
    return this->forLoopStatement;
}

const FunctionDeclaration& Statement::getFunctionDeclaration() const {
    return this->functionDeclaration;
}
