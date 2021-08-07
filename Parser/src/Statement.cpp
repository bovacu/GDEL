#include "Parser/include/Statement.h"
#include "Parser/include/Parser.h"

Statement::Statement() {
    statementsFuncs[_LEFT_COLLIBRACE] = BIND_FN(blockStatement.getFunc());
    statementsFuncs[_VAR] = BIND_FN(variableStatement.getFunc());
    statementsFuncs[_IF] = BIND_FN(ifStatement.getFunc());
    statementsFuncs[_LOOP] = BIND_FN(loopStatement.getFunc());
    statementsFuncs[_FOR] = BIND_FN(forLoopStatement.getFunc());
    statementsFuncs[_FUNC] = BIND_FN(functionStatement.getFunc());
    statementsFuncs[_RET] = BIND_FN(functionStatement.getRetFunc());
    statementsFuncs[_STRUCT] = BIND_FN(structStatement.getFunc());
    statementsFuncs[_IMPORT] = BIND_FN(importStatement.getFunc());
}


json Statement::getAst(Parser& _parser, json& _tokenToCheck) const {
    auto _tokenType = _tokenToCheck["type"].get<std::string>();
    if(statementsFuncs.find(_tokenType) != statementsFuncs.end())
        return statementsFuncs.at(_tokenType)(*this, _parser, _parser.getLookAhead());

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

const FunctionStatement& Statement::getFunctionStatement() const {
    return this->functionStatement;
}


const StructStatement& Statement::getStructStatement() const {
    return this->structStatement;
}


const ImportStatement& Statement::getImportStatement() const {
    return this->importStatement;
}

