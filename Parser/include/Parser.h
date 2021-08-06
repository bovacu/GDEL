#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <sstream>
#include <string>

#include "json.hpp"
#include "Parser/include/Tokenizer.h"
#include "Parser/include/Exception.hpp"

using namespace nlohmann;

class Parser {
    private:
        Tokenizer tokenizer;
        json lookAhead;

    friend class Tokenizer;

    public:
        json parse(const char* _code);
        json start(const char* _code);
        json eatToken(const char* _tokenType);
        int getCurrentParsinLine() const;
        const std::string getCurrentLookAheadType() const;
        json& getLookAhead();

        bool isEof() const;

        json statements();
};

#endif //PARSER_H