#ifndef __BLOCKSTATEMENT_H__
#define __BLOCKSTATEMENT_H__

#include "Parser/core/Declarations.h"

class Parser;
class Statement;

class BlockStatement {

    public:
        json getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const;
};

#endif // __BLOCKSTATEMENT_H__