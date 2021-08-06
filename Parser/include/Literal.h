#ifndef LITERAL_H
#define LITERAL_H

#include "Parser/include/Declarations.h"
#include "Parser/include/Defines.h"

class Parser;
class Statement;

class Literal {
    public:
        json getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const;
        bool isLiteral(json& _tokenToCheck) const;

    private:
        json numberAst(Parser* _parser, json& _tokenToCheck) const;
        json stringAst(Parser* _parser, json& _tokenToCheck) const;
        json boolAst(Parser* _parser, json& _tokenToCheck) const;
        json nullAst(Parser* _parser, json& _tokenToCheck) const;
        json idAst(Parser* _parser, json& _tokenToCheck) const;
        json symbolAst(Parser* _parser, json& _tokenToCheck) const;
};

#endif //LITERAL_H