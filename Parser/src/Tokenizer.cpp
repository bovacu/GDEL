#include "Parser/include/Tokenizer.h"
#include "Parser/include/Exception.hpp"
#include "Parser/include/Parser.h"
#include <sstream>

void Tokenizer::init(const char* _code) {
    this->codeLenth = strlen(_code);
    this->code = _code;
    this->charPointer = 0;
    this->linePointer = 0;
}

bool Tokenizer::hasMoreTokens() {
    return this->charPointer < this->codeLenth;
}

json Tokenizer::getNextToken() {
    if(!hasMoreTokens())
        return json{ {"type", _EOF}, {"value", _EOF} };

    // Get the type of token
    json _token;
    _token = this->tokenStripper.getTokenType(this->code, this->charPointer, this->linePointer);  

    // Unexpected token
    if(_token.size() == 0)
        throw UnexpectedTokenException(this->code[this->charPointer], this->linePointer);

    // React depending on the type of token
    auto _tokenType = _token["type"].get<std::string>();
    if(strcmp(_tokenType.c_str(), _IGNORE) == 0) {
        // Line counter for errors
        if(_token.contains("value"))
            if(strcmp(_token["value"].get<std::string>().c_str(), _N) == 0 || 
               strcmp(_token["value"].get<std::string>().c_str(), _R) == 0)
                this->linePointer++;

        return getNextToken();
    } else if(strcmp(_tokenType.c_str(), _ERROR) != 0)
        return _token;

    return json {
        {"type", _EOF},
        {"value", _EOF}
    };
}

json Tokenizer::get(Parser& _parser) {
    return this->statement.getAst(_parser, _parser.lookAhead);
}
