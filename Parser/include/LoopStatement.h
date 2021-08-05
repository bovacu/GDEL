#ifndef __WHILESTATEMENT_H__
#define __WHILESTATEMENT_H__

#include "Parser/include/Declarations.h"
class Statement;
class Parser;

class LoopStatement {
    public:
        json getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const;
};

#endif // __WHILESTATEMENT_H__