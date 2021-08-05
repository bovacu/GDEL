#ifndef __VARIABLESTATEMENT_H__
#define __VARIABLESTATEMENT_H__

#include "Parser/include/Declarations.h"

class Expression;
class Literal;
class Parser;

class VariableStatement {
    public:
        json getAst(const Expression& _literal, Parser* _parser, json& _tokenToCheck) const;

    private:
        std::vector<json> getDeclarationList(const Expression& _literal, Parser* _parser, json& _tokenToCheck) const;
        json getVariableDeclaration(const Expression& _literal, Parser* _parser, json& _tokenToCheck) const;
        json getInitializer(const Expression& _literal, Parser* _parser, json& _tokenToCheck) const;
};

#endif // __VARIABLESTATEMENT_H__