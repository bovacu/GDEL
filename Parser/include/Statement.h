#ifndef __STATEMENT_H__
#define __STATEMENT_H__

#include "Parser/include/Declarations.h"
#include "Parser/include/ExpressionStatement.h"
#include "Parser/include/BlockStatement.h"

class Parser;

class Statement {
    private:
        ExpressionStatement expressionStatement;
        BlockStatement blockStatement;

    public:
        json getAst(Parser* _parser, json& _tokenToCheck) const;
};

#endif // __STATEMENT_H__