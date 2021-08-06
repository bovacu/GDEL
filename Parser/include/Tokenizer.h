#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "Parser/include/Declarations.h"
#include "Parser/include/Statement.h"
#include "Parser/include/TokenStripper.h"

#include <iostream>
#include <ostream>
#include <string>
#include <map>

class Parser;

class Tokenizer {
    private:
        std::map<MatchCharFunc, TokenizerFunc> tokenizerMap;

    private:
        int charPointer;
        const char* code;
        int codeLenth;
        int linePointer;

        TokenStripper tokenStripper;
        Statement statement;

        friend class Parser;

    public:
        void init(const char* _code);
        bool hasMoreTokens();
        json getNextToken();
        json get(Parser& _parser);
};

#endif // TOKENIZER_H