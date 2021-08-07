#ifndef __FORLOOPSTATEMENT_H__
#define __FORLOOPSTATEMENT_H__

#include "Parser/include/IStatement.h"

class ForLoopStatement : public IStatement{
    public:
        json getAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
        inline Func getFunc() override { return BIND_FN(ForLoopStatement::getAst); }
};

#endif // __FORLOOPSTATEMENT_H__