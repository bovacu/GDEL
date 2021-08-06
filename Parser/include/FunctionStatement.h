#ifndef __FUNCTIONDECLARATION_H__
#define __FUNCTIONDECLARATION_H__

#include "Parser/include/Declarations.h"

class Parser;
class Statement;

class FunctionStatement {
    public:
        json getAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
        json getReturnAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
        std::vector<json> getParamList(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
};


#endif // __FUNCTIONDECLARATION_H__