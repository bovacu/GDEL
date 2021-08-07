#ifndef __IFSTATEMENT_H__
#define __IFSTATEMENT_H__

#include "Parser/include/IStatement.h"

class IfStatement : public IStatement{
    public:
        json getAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
        inline Func getFunc() override { return BIND_FN(IfStatement::getAst); }
};

#endif // __IFSTATEMENT_H__