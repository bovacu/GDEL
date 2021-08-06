#ifndef __STATEMENT_H__
#define __STATEMENT_H__

#include "Parser/include/ExpressionStatement.h"
#include "Parser/include/BlockStatement.h"
#include "Parser/include/VariableStatement.h"
#include "Parser/include/IfStatement.h"
#include "Parser/include/LoopStatement.h"
#include "Parser/include/ForLoopStatement.h"
#include "Parser/include/FunctionStatement.h"
#include "Parser/include/StructStatement.h"

class Parser;

class Statement {
    private:
        ExpressionStatement expressionStatement;
        BlockStatement blockStatement;
        VariableStatement variableStatement;
        IfStatement ifStatement;
        LoopStatement loopStatement;
        ForLoopStatement forLoopStatement;
        FunctionStatement functionDeclaration;
        StructStatement structStatement;

    public:
        json getAst(Parser& _parser, json& _tokenToCheck) const;
        const ExpressionStatement& getExpressionStatement() const;
        const VariableStatement& getVariableStatement() const;
        const BlockStatement& getBlockStatement() const;
        const IfStatement& getIfStatement() const;
        const LoopStatement& getLoopStatement() const;
        const StructStatement& getStructStatement() const;
        const ForLoopStatement& getForLoopStatement() const;
        const FunctionStatement& getFunctionStatement() const;
};

#endif // __STATEMENT_H__