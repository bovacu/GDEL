#ifndef __STRUCTSTATEMENT_H__
#define __STRUCTSTATEMENT_H__

#include "Parser/include/Declarations.h"

class Parser;
class Statement;

class StructStatement {
    public:
        json getAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
        json getSelfAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
        json getBaseAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
        json getNewAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
};

#endif // __STRUCTSTATEMENT_H__