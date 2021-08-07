#ifndef __IMPORTSTATEMENT_H__
#define __IMPORTSTATEMENT_H__

#include "Parser/include/IStatement.h"

class ImportStatement : public IStatement {
    public:
        json getAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
        inline Func getFunc() override { return BIND_FN(ImportStatement::getAst); }
};

#endif // __IMPORTSTATEMENT_H__