#ifndef LITERAL_H
#define LITERAL_H

#include "Parser/include/Declarations.h"
class Parser;

class Literal {
    public:
        json getAst(Parser* _parser, json& _tokenToCheck) const;
        bool isLiteral(json& _tokenToCheck) const;

    private:
        json numberAst(Parser* _parser, json& _tokenToCheck) const;
        json stringAst(Parser* _parser, json& _tokenToCheck) const;
        json boolAst(Parser* _parser, json& _tokenToCheck) const;
        json idAst(Parser* _parser, json& _tokenToCheck) const;
        json symbolAst(Parser* _parser, json& _tokenToCheck) const;
};

#endif //LITERAL_H