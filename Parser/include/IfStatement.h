#ifndef __IFSTATEMENT_H__
#define __IFSTATEMENT_H__

#include "Parser/include/Declarations.h"
class Statement;
class Parser;

class IfStatement {
    public:
        json getAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
};

#endif // __IFSTATEMENT_H__