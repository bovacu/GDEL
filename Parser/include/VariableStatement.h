#ifndef __VARIABLESTATEMENT_H__
#define __VARIABLESTATEMENT_H__

#include "Parser/include/Declarations.h"

class Expression;
class Literal;
class Parser;
class Statement;

class VariableStatement {
    public:
        json getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const;
        std::vector<json> getDeclarationList(const Expression& _expression, Parser* _parser, json& _tokenToCheck, bool _isSpecialDecl = false) const;
        
    private:
        json getVariableDeclaration(const Expression& _expression, Parser* _parser, json& _tokenToCheck, bool _isSpecialDecl = false) const;
        json getInitializer(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const;
};

#endif // __VARIABLESTATEMENT_H__