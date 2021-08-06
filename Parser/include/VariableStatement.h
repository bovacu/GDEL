#ifndef __VARIABLESTATEMENT_H__
#define __VARIABLESTATEMENT_H__

#include "Parser/include/Declarations.h"

class Parser;
class Statement;
class Expression;

class VariableStatement {
    public:
        json getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const;
        std::vector<json> getDeclarationList(const Statement& _statement, const Expression& _expression, Parser* _parser, json& _tokenToCheck, bool _isSpecialDecl = false) const;
        
    private:
        json getVariableDeclaration(const Statement& _statement, const Expression& _expression, Parser* _parser, json& _tokenToCheck, bool _isSpecialDecl = false) const;
        json getInitializer(const Statement& _statement, const Expression& _expression, Parser* _parser, json& _tokenToCheck) const;
};

#endif // __VARIABLESTATEMENT_H__