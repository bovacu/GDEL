#ifndef __BLOCKSTATEMENT_H__
#define __BLOCKSTATEMENT_H__

#include "Parser/include/IStatement.h"

class BlockStatement : public IStatement {

    public:
        json getAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
        inline Func getFunc() override { return BIND_FN(BlockStatement::getAst); }
};

#endif // __BLOCKSTATEMENT_H__