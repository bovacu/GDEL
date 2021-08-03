#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "Parser/core/Declarations.h"
#include "Parser/elements/statement/Statement.h"

#include <iostream>
#include <ostream>
#include <string>
#include <map>

class Ignore;
class Parser;

class Tokenizer {
    private:
        std::map<MatchCharFunc, TokenizerFunc> tokenizerMap;

    private:
        static constexpr const char* tokenTypeNames[] = {"PROGRAM", "INTEGER", "FLOAT", "BOOL", "STRING", "IGNORE", "ERROR", "EOF"};

    private:
        int charPointer;
        const char* code;
        int codeLenth;
        int linePointer;

        Statement statement;
        Ignore*  ignore;

        friend class Parser;

    public:
        Parser* parser;

    public:
        ~Tokenizer();
        void init(const char* _code);
        bool hasMoreTokens();
        json getNextToken();
        json get(Parser* _parser);
};

#endif // TOKENIZER_H