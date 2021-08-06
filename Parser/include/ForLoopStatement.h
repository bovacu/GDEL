#ifndef __FORLOOPSTATEMENT_H__
#define __FORLOOPSTATEMENT_H__

#include "Parser/include/Declarations.h"
class Statement;
class Parser;
class VariableStatement;

class ForLoopStatement {
    public:
        json getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const;
};

#endif // __FORLOOPSTATEMENT_H__