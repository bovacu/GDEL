#ifndef __FUNCTIONDECLARATION_H__
#define __FUNCTIONDECLARATION_H__

#include "Parser/include/Declarations.h"

class Expression;
class Parser;
class Statement;

class FunctionDeclaration {
    public:
        json getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const;
        json getReturnAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const;
    
    private:
        std::vector<json> getParamList(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const;
};


#endif // __FUNCTIONDECLARATION_H__