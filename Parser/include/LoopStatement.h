#ifndef __WHILESTATEMENT_H__
#define __WHILESTATEMENT_H__

#include "Parser/include/IStatement.h"

class LoopStatement : public IStatement {
    public:
        json getAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
        inline Func getFunc() override { return BIND_FN(LoopStatement::getAst); }
};

#endif // __WHILESTATEMENT_H__