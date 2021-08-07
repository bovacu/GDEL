#ifndef __FUNCTIONDECLARATION_H__
#define __FUNCTIONDECLARATION_H__

#include "Parser/include/IStatement.h"

class FunctionStatement : public IStatement{
    public:
        json getAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
        json getReturnAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
        std::vector<json> getParamList(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
        inline Func getRetFunc() { return BIND_FN(FunctionStatement::getReturnAst); }
        inline Func getFunc() override { return BIND_FN(FunctionStatement::getAst); }
};


#endif // __FUNCTIONDECLARATION_H__