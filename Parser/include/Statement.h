#ifndef __STATEMENT_H__
#define __STATEMENT_H__

#include "Parser/include/Declarations.h"
#include "Parser/include/ExpressionStatement.h"
#include "Parser/include/BlockStatement.h"
#include "Parser/include/VariableStatement.h"
#include "Parser/include/IfStatement.h"

class Parser;

class Statement {
    private:
        ExpressionStatement expressionStatement;
        BlockStatement blockStatement;
        VariableStatement variableStatement;
        IfStatement ifStatement;

    public:
        json getAst(Parser* _parser, json& _tokenToCheck) const;
        const ExpressionStatement& getExpressionStatement() const;
};

#endif // __STATEMENT_H__