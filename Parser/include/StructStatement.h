#ifndef __STRUCTSTATEMENT_H__
#define __STRUCTSTATEMENT_H__

#include "Parser/include/IStatement.h"

class StructStatement : public IStatement {
    public:
        json getAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
        json getSelfAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
        json getBaseAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
        json getNewAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
        inline Func getFunc() override { return BIND_FN(StructStatement::getAst); }
};

#endif // __STRUCTSTATEMENT_H__